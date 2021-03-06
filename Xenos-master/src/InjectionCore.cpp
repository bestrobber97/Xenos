#include "InjectionCore.h"
#include <BlackBone/src/BlackBone/Process/RPC/RemoteFunction.hpp>
#include <iterator>

InjectionCore::InjectionCore( HWND& hMainDlg )
    : _hMainDlg( hMainDlg )
{
    // Load driver
    //blackbone::Driver().EnsureLoaded();
}


InjectionCore::~InjectionCore()
{
    //
    // If at least one process with allocated physical pages exist, prevent driver unload or process will crash
    // Although PID can be reused, I'm too lazy to implement more reliable detection
    //
    std::vector<DWORD> existing = blackbone::Process::EnumByName( L"" ), mutual;
    std::sort( existing.begin(), existing.end() );
    std::sort( _criticalProcList.begin(), _criticalProcList.end() );
    std::set_intersection( existing.begin(), existing.end(), _criticalProcList.begin(), _criticalProcList.end(), std::back_inserter( mutual ) );
    if (mutual.empty())
        blackbone::Driver().Unload();
}


/// <summary>
/// Get injection target
/// </summary>
/// <param name="context">Injection context.</param>
/// <param name="pi">Process info in case of new process</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::GetTargetProcess( InjectContext& context, PROCESS_INFORMATION& pi )
{
    NTSTATUS status = ERROR_SUCCESS;

    // No process required for driver mapping
    if (context.cfg.injectMode == Kernel_DriverMap)
        return status;

    // Await new process
    if (context.cfg.processMode == ManualLaunch)
    {
        xlog::Normal( "Waiting on process %ls", context.procPath.c_str() );

        auto procName = blackbone::Utils::StripPath( context.procPath );
        if (procName.empty())
        {
            Message::ShowWarning( _hMainDlg, L"Please select executable to wait for\n" );
            return STATUS_NOT_FOUND;
        }
        
        // Filter already existing processes
        std::vector<blackbone::ProcessInfo> newList;

        if (context.procList.empty())
            context.procList = blackbone::Process::EnumByNameOrPID( 0, procName ).result( std::vector<blackbone::ProcessInfo>() );

        // Wait for process
        for (context.waitActive = true;; Sleep( 10 ))
        {
            // Canceled by user
            if (!context.waitActive)
            {
                xlog::Warning( "Process wait canceled by user" );
                return STATUS_REQUEST_ABORTED;
            }

            if (!context.procDiff.empty())
            {
                context.procList = newList;

                // Skip first N found processes
                if (context.skippedCount < context.cfg.skipProc)
                {
                    context.skippedCount++;
                    context.procDiff.erase( context.procDiff.begin() );

                    continue;
                }

                context.pid = context.procDiff.front().pid;
                context.procDiff.erase( context.procDiff.begin() );

                xlog::Verbose( "Got process %d", context.pid );
                break;
            }
            else
            {  
                // Detect new process
                newList = blackbone::Process::EnumByNameOrPID( 0, procName ).result( std::vector<blackbone::ProcessInfo>() );
                std::set_difference( 
                    newList.begin(), newList.end(), 
                    context.procList.begin(), context.procList.end(), 
                    std::inserter( context.procDiff, context.procDiff.begin() ) 
                    );
            }
        }
    }
    // Create new process
    else if (context.cfg.processMode == NewProcess)
    {
        STARTUPINFOW si = { 0 };
        si.cb = sizeof( si );

        xlog::Normal( "Creating new process '%ls' with arguments '%ls'", context.procPath.c_str(), context.cfg.procCmdLine.c_str() );

        if (!CreateProcessW( context.procPath.c_str(), (LPWSTR)context.cfg.procCmdLine.c_str(),
            NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, blackbone::Utils::GetParent( context.procPath ).c_str(), &si, &pi ))
        {
            Message::ShowError( _hMainDlg, L"Failed to create new process.\n" + blackbone::Utils::GetErrorDescription( LastNtStatus() ) );
            return LastNtStatus();
        }

        // Escalate handle access rights through driver
        if (context.cfg.krnHandle)
        {
            xlog::Normal( "Escalating process handle access" );

            status = _process.Attach( pi.dwProcessId, PROCESS_QUERY_LIMITED_INFORMATION );
            if (NT_SUCCESS( status ))
            {
                status = blackbone::Driver().PromoteHandle(
                    GetCurrentProcessId(),
                    _process.core().handle(),
                    DEFAULT_ACCESS_P | PROCESS_QUERY_LIMITED_INFORMATION
                    );
            }
        }
        else
        {
            // Attach for thread init
            status = _process.Attach( pi.dwProcessId );
        }

        // Create new thread to make sure LdrInitializeProcess gets called
        if (NT_SUCCESS( status ))
            _process.EnsureInit();

        if (!NT_SUCCESS( status ))
        {
            xlog::Error( "Failed to attach to process, status 0x%X", status );
            return status;
        }

        // No process handle is required for kernel injection
        if (context.cfg.injectMode >= Kernel_Thread)
        {
            context.pid = pi.dwProcessId;
            _process.Detach();

            // Thread must be running for APC execution
            if (context.cfg.injectMode == Kernel_APC)
            {
                ResumeThread( pi.hThread );
                Sleep( 100 );
            }
        }

        // Because the only suitable thread is suspended, thread hijacking can't be used
        context.cfg.hijack = false;
    }
    // Attach to existing process
    if (context.cfg.injectMode < Kernel_Thread && context.cfg.processMode != NewProcess)
    {
        // Escalate handle access rights through driver
        if (context.cfg.krnHandle)
        {
            xlog::Normal( "Escalating process handle access" );

            status = _process.Attach( context.pid, PROCESS_QUERY_LIMITED_INFORMATION );
            if (NT_SUCCESS( status ))
            {
                status = blackbone::Driver().PromoteHandle(
                    GetCurrentProcessId(),
                    _process.core().handle(),
                    DEFAULT_ACCESS_P | PROCESS_QUERY_LIMITED_INFORMATION
                    );
            }

            if (!NT_SUCCESS( status ))
                xlog::Error( "Failed to escalate process handle access, status 0x%X", status );
        }
        else
            status = _process.Attach( context.pid );

        if (NT_SUCCESS( status ) && !_process.valid())
            status = STATUS_PROCESS_IS_TERMINATING;

        if (status != STATUS_SUCCESS)
        {
            std::wstring errmsg = L"Can not attach to the process.\n" + blackbone::Utils::GetErrorDescription( status );
            Message::ShowError( _hMainDlg, errmsg );

            return status;
        }


        //
        // Make sure loader is initialized
        //
        bool ldrInit = false;
        if (_process.core().isWow64())
        {
            blackbone::_PEB32 peb32 = { 0 };
            _process.core().peb32( &peb32 );
            ldrInit = peb32.Ldr != 0;
        }
        else
        {
            blackbone::_PEB64 peb64 = { 0 };
            _process.core().peb64( &peb64 );
            ldrInit = peb64.Ldr != 0;
        }

        // Create new thread to make sure LdrInitializeProcess gets called
        if (!ldrInit)
            _process.EnsureInit();
    }

    return ERROR_SUCCESS;
}

using namespace std;

class ilumoer {
public:
	bool xdmgblol;
	bool ytshbewkxbzfo;
	ilumoer();
	string xjwlzkpntoon(double tkltsylqpjsnytz, int frdjjamirbfrmz, double tkmvvt);
	int lpssaqzackqwhgtc(string htlgeqcraaewi, bool nygzhitnmhrkat, int jbfpwaj, double ejqkioaclophw, double wftanttytrypym, bool nrplt, bool zktkfvuctgryy, bool ovrvneewxkovo, int ozpajpt);
	double tpvzargfrpbyuknxuqb(double naohpyjfaiusj, int bhleabo, double xhqmhwxydqr, int fmllj, int zslpibxgpyarga, string zjmokzuaidyp, int jttvjlcquk);
	int rzrrqcjkgnqd(double taubyabdghjodzv);
	int nrmgtgymovhhroucvitfuvr(bool sdbnziocqmco, double njqpitao, double uzzvjrct, double qswbsdhfowy, string hsriqia);
	bool ynydsuwccynvbjbwhcn();
	double sjiufhbgrzzthqxpeb(string funfhel, bool rynycpwts);

protected:
	string fttfqgukwaznm;
	string vsvhidplahphpn;
	bool uxmxmaiukspf;

	int ltjedltxrahd(bool ngrijywruathkxj, bool sukbiqjdiciax, string vdvtuzm, bool luekoo, double xzhvjbjvvdtum, bool cobsnbgw, double pjylrkfj, bool kvemaiij);
	string cwzgceyygmxpswbifxudjlyl();
	bool drrgdvqigdscnyjdhtlivkl(string nmpjhice, int cdcncodlpco, bool amsezblngziwxzx, bool glwaokn, bool ktmwafo, string typbciukfo);
	bool acapwmrntikrtvw(string vohwpxpliozcaw, string rondya, double ycxeh, string ikdjtbfsb, string vlfvlk);
	void wctmwvgtyrd(bool kisnmauzrvrths, int gllller, bool xubig, bool viakaqaii, int vvwekpcnkubxc, string brbrkdus);
	void sqfyicwpyjangqhp(double bjlvgkljhtk, bool xtpoglbffdyk, bool wkdydox, int afnwbwlf, int pamvkvwlehr, int lzylposnsheh);
	int ylwhxpdkngsweyk(bool lkimdgkwibfcm, bool aufbnwpzwou, int zmlenpqpqszbp, double qqnva, string dujgyotxk, bool akwflgufiaiaoj, string flnvlvtlbfnofd);
	int ardniygqztrni(double tjmkrxmngppjeph, bool qxvgiyyutup);
	int usrhxblghktyuufyhqt(string gfngtlaaplcofv, int ibydekwntaxrhq, double lwvpyhrmgfdidm, double pocktqwvlptax, string hrlxdtbpamiaa, string rfffthgmnxcr, string dtjqyfy, bool sjyqjtxfoo, double hwqkahzpklsw);

private:
	double vkpoxr;
	string pqylmpipnw;
	string ewmrlx;
	string bzcwotmndwukfy;

	int ofhizftlbazis(string pjvjkq, bool hvbmnreneoaswi, bool puxzdipac, bool vwvtkvjmna, string mswyl, double wfkrdff, bool nmvzzcyy, int mgzbtq);
	double ptnjfyoqwqc(int deuzlevfo, double kffjkpfwfk);

};
int ilumoer::ofhizftlbazis(string pjvjkq, bool hvbmnreneoaswi, bool puxzdipac, bool vwvtkvjmna, string mswyl, double wfkrdff, bool nmvzzcyy, int mgzbtq) {
	double gzbhjfurenuvhzl = 75659;
	int kamedhs = 718;
	string pbpwrueh = "sqqmmnytidfviemthaalulkbndglllmvepzfgagcivshtneeztlwossekgvwfkzvuxmlsbzvacfhfqxosiqpfg";
	int rpwwracfpswdkgc = 3430;
	if (string("sqqmmnytidfviemthaalulkbndglllmvepzfgagcivshtneeztlwossekgvwfkzvuxmlsbzvacfhfqxosiqpfg") != string("sqqmmnytidfviemthaalulkbndglllmvepzfgagcivshtneeztlwossekgvwfkzvuxmlsbzvacfhfqxosiqpfg")) {
		int mgl;
		for (mgl = 8; mgl > 0; mgl--) {
			continue;
		}
	}
	if (718 != 718) {
		int gayhd;
		for (gayhd = 71; gayhd > 0; gayhd--) {
			continue;
		}
	}
	if (string("sqqmmnytidfviemthaalulkbndglllmvepzfgagcivshtneeztlwossekgvwfkzvuxmlsbzvacfhfqxosiqpfg") == string("sqqmmnytidfviemthaalulkbndglllmvepzfgagcivshtneeztlwossekgvwfkzvuxmlsbzvacfhfqxosiqpfg")) {
		int qpbct;
		for (qpbct = 19; qpbct > 0; qpbct--) {
			continue;
		}
	}
	return 82581;
}

double ilumoer::ptnjfyoqwqc(int deuzlevfo, double kffjkpfwfk) {
	string ejbziwfre = "unzfvkpdlrjnyhxfwlgdeokeovtocwzuusfhgfmmbsgc";
	int zonypshdsjfbrwz = 2469;
	int zbflviqsdmhg = 4979;
	bool louzoh = false;
	double rfwzzafvh = 62454;
	int ryqpfqfnqstjyp = 3703;
	string uguvyjdvf = "syvnxdtmaosekwyectqu";
	double fdpldpnxnepee = 68880;
	string ltskhdv = "pnknqxsdnzcleanekgz";
	if (68880 != 68880) {
		int zykfhiodhj;
		for (zykfhiodhj = 42; zykfhiodhj > 0; zykfhiodhj--) {
			continue;
		}
	}
	return 17899;
}

int ilumoer::ltjedltxrahd(bool ngrijywruathkxj, bool sukbiqjdiciax, string vdvtuzm, bool luekoo, double xzhvjbjvvdtum, bool cobsnbgw, double pjylrkfj, bool kvemaiij) {
	string dgbin = "ghtwmoxdyzuyoulfxhzsdhtbdrjlbuuiwmlsztkz";
	string ayozwgsr = "usqzigadvthvdmboovnzygaebphfuadddlombzwqlohfbomeskxppdkjijtkkwlsyiirxcjxzblviosjhwqof";
	if (string("ghtwmoxdyzuyoulfxhzsdhtbdrjlbuuiwmlsztkz") != string("ghtwmoxdyzuyoulfxhzsdhtbdrjlbuuiwmlsztkz")) {
		int sd;
		for (sd = 49; sd > 0; sd--) {
			continue;
		}
	}
	if (string("ghtwmoxdyzuyoulfxhzsdhtbdrjlbuuiwmlsztkz") != string("ghtwmoxdyzuyoulfxhzsdhtbdrjlbuuiwmlsztkz")) {
		int hylq;
		for (hylq = 28; hylq > 0; hylq--) {
			continue;
		}
	}
	return 12843;
}

string ilumoer::cwzgceyygmxpswbifxudjlyl() {
	bool sxpnjajpbygzy = true;
	if (true != true) {
		int jop;
		for (jop = 82; jop > 0; jop--) {
			continue;
		}
	}
	if (true == true) {
		int ln;
		for (ln = 94; ln > 0; ln--) {
			continue;
		}
	}
	return string("yyvzyohfmfkhcznp");
}

bool ilumoer::drrgdvqigdscnyjdhtlivkl(string nmpjhice, int cdcncodlpco, bool amsezblngziwxzx, bool glwaokn, bool ktmwafo, string typbciukfo) {
	double yhhynafydwi = 3323;
	int rqjidis = 4335;
	string jxztxwnarnss = "avhpktynbfksszdqrgavdgitqtzpzhuybxfefzkbtekkd";
	string hplzatfndf = "vllmqashoogsjjjapmygvnnpzoefkfyebwahsklihovtznimacbvtnsejmxnrsecvbrwtqgxd";
	int jyuqgqrq = 1393;
	if (string("vllmqashoogsjjjapmygvnnpzoefkfyebwahsklihovtznimacbvtnsejmxnrsecvbrwtqgxd") != string("vllmqashoogsjjjapmygvnnpzoefkfyebwahsklihovtznimacbvtnsejmxnrsecvbrwtqgxd")) {
		int fjhfodk;
		for (fjhfodk = 78; fjhfodk > 0; fjhfodk--) {
			continue;
		}
	}
	if (string("vllmqashoogsjjjapmygvnnpzoefkfyebwahsklihovtznimacbvtnsejmxnrsecvbrwtqgxd") == string("vllmqashoogsjjjapmygvnnpzoefkfyebwahsklihovtznimacbvtnsejmxnrsecvbrwtqgxd")) {
		int jv;
		for (jv = 61; jv > 0; jv--) {
			continue;
		}
	}
	return false;
}

bool ilumoer::acapwmrntikrtvw(string vohwpxpliozcaw, string rondya, double ycxeh, string ikdjtbfsb, string vlfvlk) {
	bool izwyrksubdy = false;
	double aqoomdaqfq = 42972;
	if (42972 != 42972) {
		int toybljfaey;
		for (toybljfaey = 61; toybljfaey > 0; toybljfaey--) {
			continue;
		}
	}
	return true;
}

void ilumoer::wctmwvgtyrd(bool kisnmauzrvrths, int gllller, bool xubig, bool viakaqaii, int vvwekpcnkubxc, string brbrkdus) {
	int ozrgipbbqlkx = 4057;
	bool rtbnvavzktwvmim = false;
	double fljonfneeif = 2493;
	double lwixj = 13320;
	string ckszhlludctsn = "kxlahsimfebxcoftmlczjyxceirrypfsbyniixbgugidfcpwstowobrslntvcynxhwmdjabzkiaggqnm";
	bool idetviqamvdou = true;
	int omrrm = 1099;

}

void ilumoer::sqfyicwpyjangqhp(double bjlvgkljhtk, bool xtpoglbffdyk, bool wkdydox, int afnwbwlf, int pamvkvwlehr, int lzylposnsheh) {
	int akneevlooqp = 3620;
	int unzjtlbonxdbfz = 1776;
	bool sgpbyfbbwivh = false;
	string ugcndhwdcjwljes = "dtlqkynujinjfvssvduiejjpdmxijvahgapodrgkvleepsgpwrrxypxsxtyclgybwhtvnqqgecjoiglngoublpzvuaf";
	bool lilfabjt = true;
	int cdxrfdodmee = 3086;
	int rxptl = 5055;
	int ixgkwjeqeoo = 1288;
	string khzdcpqzevikdsk = "vucztbutlxyqzcxexwqdhqblniuulnpoduzpzibeiaxroxpsxtcfcifxtewamwibirjpchdwiwfuftrfsmbufzmmpxzfwffizhx";
	if (1288 != 1288) {
		int hwsgv;
		for (hwsgv = 34; hwsgv > 0; hwsgv--) {
			continue;
		}
	}

}

int ilumoer::ylwhxpdkngsweyk(bool lkimdgkwibfcm, bool aufbnwpzwou, int zmlenpqpqszbp, double qqnva, string dujgyotxk, bool akwflgufiaiaoj, string flnvlvtlbfnofd) {
	bool smueqdiezkjlvcr = false;
	string lnykryxi = "jlwdohvouountdnlmgihceanvdmwdotepvhxxvnjmd";
	double fkvrdfylrgydyb = 6624;
	string gmxfdnekgrzt = "ocihfevodzwsvjllwnoaldbizbgzhfrtfcyuepdc";
	double wxsnicdeyfoph = 4028;
	int hebpkgoppjvup = 3840;
	double jybisuzxkkgnbt = 31089;
	int mrqeyfofog = 3258;
	string haskpxaymtgyi = "zisywiidepuzgnwmfjksmzwheovyblbdpwitxy";
	bool miexhqmatqgdxrb = false;
	if (3258 != 3258) {
		int mpqhunmfcx;
		for (mpqhunmfcx = 35; mpqhunmfcx > 0; mpqhunmfcx--) {
			continue;
		}
	}
	if (false != false) {
		int nknyijdygh;
		for (nknyijdygh = 15; nknyijdygh > 0; nknyijdygh--) {
			continue;
		}
	}
	if (31089 == 31089) {
		int ykgoktx;
		for (ykgoktx = 25; ykgoktx > 0; ykgoktx--) {
			continue;
		}
	}
	if (false == false) {
		int wpq;
		for (wpq = 97; wpq > 0; wpq--) {
			continue;
		}
	}
	if (3258 == 3258) {
		int shdkyu;
		for (shdkyu = 100; shdkyu > 0; shdkyu--) {
			continue;
		}
	}
	return 23668;
}

int ilumoer::ardniygqztrni(double tjmkrxmngppjeph, bool qxvgiyyutup) {
	double lgfnuatfq = 28285;
	string hakgjk = "dcsqdvfyzclkjdwmbdtnitxhnndcjofhwadiybtgdjn";
	double fquhbnqdc = 4504;
	bool kndizghpyfe = false;
	if (28285 != 28285) {
		int bzisgew;
		for (bzisgew = 95; bzisgew > 0; bzisgew--) {
			continue;
		}
	}
	if (false != false) {
		int mcli;
		for (mcli = 86; mcli > 0; mcli--) {
			continue;
		}
	}
	if (28285 == 28285) {
		int jdyzba;
		for (jdyzba = 93; jdyzba > 0; jdyzba--) {
			continue;
		}
	}
	if (28285 != 28285) {
		int zvcu;
		for (zvcu = 52; zvcu > 0; zvcu--) {
			continue;
		}
	}
	if (false != false) {
		int pead;
		for (pead = 78; pead > 0; pead--) {
			continue;
		}
	}
	return 75313;
}

int ilumoer::usrhxblghktyuufyhqt(string gfngtlaaplcofv, int ibydekwntaxrhq, double lwvpyhrmgfdidm, double pocktqwvlptax, string hrlxdtbpamiaa, string rfffthgmnxcr, string dtjqyfy, bool sjyqjtxfoo, double hwqkahzpklsw) {
	string qpbuxnlacoigv = "ayeynwspmolomunzunxfwfisasdsjpjnapltolleqncurrfyswgzknmpeakrzteyjqtzwejteiqvrkayngjleinvkuhzoualuy";
	double ezvunh = 25271;
	if (25271 != 25271) {
		int ihuemr;
		for (ihuemr = 58; ihuemr > 0; ihuemr--) {
			continue;
		}
	}
	if (string("ayeynwspmolomunzunxfwfisasdsjpjnapltolleqncurrfyswgzknmpeakrzteyjqtzwejteiqvrkayngjleinvkuhzoualuy") != string("ayeynwspmolomunzunxfwfisasdsjpjnapltolleqncurrfyswgzknmpeakrzteyjqtzwejteiqvrkayngjleinvkuhzoualuy")) {
		int sxxxsw;
		for (sxxxsw = 23; sxxxsw > 0; sxxxsw--) {
			continue;
		}
	}
	if (string("ayeynwspmolomunzunxfwfisasdsjpjnapltolleqncurrfyswgzknmpeakrzteyjqtzwejteiqvrkayngjleinvkuhzoualuy") == string("ayeynwspmolomunzunxfwfisasdsjpjnapltolleqncurrfyswgzknmpeakrzteyjqtzwejteiqvrkayngjleinvkuhzoualuy")) {
		int ibotzh;
		for (ibotzh = 71; ibotzh > 0; ibotzh--) {
			continue;
		}
	}
	return 64562;
}

string ilumoer::xjwlzkpntoon(double tkltsylqpjsnytz, int frdjjamirbfrmz, double tkmvvt) {
	bool frjylyexgk = true;
	if (true == true) {
		int bggue;
		for (bggue = 14; bggue > 0; bggue--) {
			continue;
		}
	}
	if (true != true) {
		int qktfa;
		for (qktfa = 53; qktfa > 0; qktfa--) {
			continue;
		}
	}
	if (true != true) {
		int owlmsexkc;
		for (owlmsexkc = 88; owlmsexkc > 0; owlmsexkc--) {
			continue;
		}
	}
	return string("ylj");
}

int ilumoer::lpssaqzackqwhgtc(string htlgeqcraaewi, bool nygzhitnmhrkat, int jbfpwaj, double ejqkioaclophw, double wftanttytrypym, bool nrplt, bool zktkfvuctgryy, bool ovrvneewxkovo, int ozpajpt) {
	bool skypv = false;
	int gqolaho = 1874;
	bool xjyokhjtykqvo = true;
	string sphrgi = "sjlecnkxaaocmigygtsjqdnpvcgbvpvflnaisrohlwnzqoyacjcrwepxuidegivfl";
	bool qerbyulcxkqv = false;
	int fnfoqdniyv = 1049;
	bool fpcoglksetebekc = false;
	if (false == false) {
		int qmkkz;
		for (qmkkz = 26; qmkkz > 0; qmkkz--) {
			continue;
		}
	}
	if (string("sjlecnkxaaocmigygtsjqdnpvcgbvpvflnaisrohlwnzqoyacjcrwepxuidegivfl") != string("sjlecnkxaaocmigygtsjqdnpvcgbvpvflnaisrohlwnzqoyacjcrwepxuidegivfl")) {
		int ceqgjiwih;
		for (ceqgjiwih = 40; ceqgjiwih > 0; ceqgjiwih--) {
			continue;
		}
	}
	if (1874 != 1874) {
		int nvy;
		for (nvy = 10; nvy > 0; nvy--) {
			continue;
		}
	}
	return 24972;
}

double ilumoer::tpvzargfrpbyuknxuqb(double naohpyjfaiusj, int bhleabo, double xhqmhwxydqr, int fmllj, int zslpibxgpyarga, string zjmokzuaidyp, int jttvjlcquk) {
	double htlvocpwm = 17307;
	int rsdtzrev = 3680;
	double rpgvoggjfwnj = 21348;
	int udsxthyecpeycy = 2590;
	if (2590 != 2590) {
		int alsxdbscib;
		for (alsxdbscib = 44; alsxdbscib > 0; alsxdbscib--) {
			continue;
		}
	}
	if (3680 != 3680) {
		int axdvjpirho;
		for (axdvjpirho = 91; axdvjpirho > 0; axdvjpirho--) {
			continue;
		}
	}
	if (17307 == 17307) {
		int lg;
		for (lg = 47; lg > 0; lg--) {
			continue;
		}
	}
	return 18717;
}

int ilumoer::rzrrqcjkgnqd(double taubyabdghjodzv) {
	string dgsswnbz = "xdwoqbpcrxmfik";
	double tfhty = 7759;
	double cxnnfkqeci = 18775;
	bool fqklm = true;
	string eaqucioqmcmhejl = "fsuhvvijwwyhqeyx";
	if (true == true) {
		int anmbblkvzb;
		for (anmbblkvzb = 69; anmbblkvzb > 0; anmbblkvzb--) {
			continue;
		}
	}
	if (7759 != 7759) {
		int yrufk;
		for (yrufk = 26; yrufk > 0; yrufk--) {
			continue;
		}
	}
	if (true != true) {
		int sz;
		for (sz = 85; sz > 0; sz--) {
			continue;
		}
	}
	if (true == true) {
		int zkckmekp;
		for (zkckmekp = 86; zkckmekp > 0; zkckmekp--) {
			continue;
		}
	}
	if (string("fsuhvvijwwyhqeyx") == string("fsuhvvijwwyhqeyx")) {
		int vebno;
		for (vebno = 63; vebno > 0; vebno--) {
			continue;
		}
	}
	return 35285;
}

int ilumoer::nrmgtgymovhhroucvitfuvr(bool sdbnziocqmco, double njqpitao, double uzzvjrct, double qswbsdhfowy, string hsriqia) {
	bool pbznnsgaierlj = true;
	bool tmmtkw = false;
	bool ggfdsuvneo = true;
	double dkfrmwmj = 25841;
	string vbmfvgjkujy = "nvrnrasibtuqfulbgfuucbmxsvnabiphllqcrmvkpzrzbjrqcr";
	double fzynbpbgfnshf = 20444;
	return 211;
}

bool ilumoer::ynydsuwccynvbjbwhcn() {
	int mmqbyvk = 4536;
	string chcpzvwlxsyarsk = "pcopckdnzxwwptknrbsktxdypqmtfspspnurebsjgqvdonxzwfgfa";
	if (string("pcopckdnzxwwptknrbsktxdypqmtfspspnurebsjgqvdonxzwfgfa") != string("pcopckdnzxwwptknrbsktxdypqmtfspspnurebsjgqvdonxzwfgfa")) {
		int dcbitqkzue;
		for (dcbitqkzue = 8; dcbitqkzue > 0; dcbitqkzue--) {
			continue;
		}
	}
	if (4536 == 4536) {
		int zuq;
		for (zuq = 46; zuq > 0; zuq--) {
			continue;
		}
	}
	if (string("pcopckdnzxwwptknrbsktxdypqmtfspspnurebsjgqvdonxzwfgfa") == string("pcopckdnzxwwptknrbsktxdypqmtfspspnurebsjgqvdonxzwfgfa")) {
		int mjbpvdbe;
		for (mjbpvdbe = 98; mjbpvdbe > 0; mjbpvdbe--) {
			continue;
		}
	}
	return true;
}

double ilumoer::sjiufhbgrzzthqxpeb(string funfhel, bool rynycpwts) {
	return 99927;
}

ilumoer::ilumoer() {
	this->xjwlzkpntoon(15022, 4492, 68036);
	this->lpssaqzackqwhgtc(string("nja"), false, 8754, 50832, 31217, true, true, false, 3008);
	this->tpvzargfrpbyuknxuqb(4114, 5175, 13529, 4854, 6092, string("rhhubbwwzoxfqpxizlclrqjbskicninpwdmdmj"), 4799);
	this->rzrrqcjkgnqd(41507);
	this->nrmgtgymovhhroucvitfuvr(true, 2764, 8976, 30351, string("dykgwatyqqgsonngwgarlvwonvnrtulvavzqdrgfnarjepulkm"));
	this->ynydsuwccynvbjbwhcn();
	this->sjiufhbgrzzthqxpeb(string("ndijtovmamhfwcjlbidlganyyloqynvzkpcaixjswilvsqlbcpterbddvaueudjnfvqoybvzplprtkqtlhpgbvqjllxskun"), false);
	this->ltjedltxrahd(false, false, string("pqwzmzjcdqqojtawcvlareml"), true, 14620, true, 46878, true);
	this->cwzgceyygmxpswbifxudjlyl();
	this->drrgdvqigdscnyjdhtlivkl(string("krzirfcjphihvqilrsykiqtfmcbsogjkovvasfziupiznqryimexwaqhsx"), 7863, false, false, false, string("nvobduragxovxkpwqkratzomwbyhmmljxusyetqpbrjlsgcqhebpqxxuq"));
	this->acapwmrntikrtvw(string("jemdwyhswvpbyvmccizkyyccnhqtqzuaaqzepwhuglrnyoykylsgtirwkrnqmomawyrcqzxsplyldtgctkmurrtsgwdw"), string("zrxwzlvapijtlyvgrvnpxyctplhkgsmttayxosvliwhyacxlcfbqgmjqcbneuukpdtehcjyhupthbeimtbbnycyoluhep"), 11049, string("mjkgwbwawsfnpfqbgxqmnnilmskblqixjlyipmifudhvzuheqljltzwvlegbsligphgepanentynsm"), string("ozbeyzdasbbyhyrsluxaabeipdowcuwptjeonolszjzxehwwaaupsemfegdgvarvihrf"));
	this->wctmwvgtyrd(false, 2141, false, false, 6981, string("bdpniqluiiigarvipxvxdvzlmcfyoxotadtoiwhpznrsxrfmdtebxwketpmoptvxqztwuarugltu"));
	this->sqfyicwpyjangqhp(1942, false, true, 371, 2561, 1337);
	this->ylwhxpdkngsweyk(true, true, 4655, 19512, string("uqjofjpmpqliwudyvvfznixgguxihjdhqutgfvergxbktgekbzcvpmevdjltzkqoo"), true, string("nwbsaymrwkrchtbwvnuxcyrcwgrdigjdbgppeww"));
	this->ardniygqztrni(1163, true);
	this->usrhxblghktyuufyhqt(string("qolxexpercwkija"), 799, 48686, 45564, string("wumvewpl"), string("rqwnrrzhymcxvbuqmmywtywskjgrbqdtzvbfnaxygkmhewasndlypysrxcgintkemmooelsqakehlsqcspyxu"), string("tapcikmisushghgxreeqhnypweheouvavisrlkhbhgep"), true, 41144);
	this->ofhizftlbazis(string("xcsogcedjgpzksgukiksmjztpzvdqmueaaulhvhcabfahizgdnevawgsttqstnlq"), false, false, true, string("qzznnbukiwceib"), 39333, false, 880);
	this->ptnjfyoqwqc(1113, 10295);
}






using namespace std;

class elnvzqr {
public:
	int tmgsn;
	int sxbwy;
	int jkdwrevvhp;
	double lwdlszkil;
	elnvzqr();
	double shsvuumcveiaqtvkxzb();
	string htnavucgdotnndbxmmr(string hfwfwunozxtzkpy);
	string ymiekhsxsxygjwz(bool girtpgt, int zwtqrjjmaezv, int zourxsl, string nvqagkvx, string pqbbczo, bool akremx, bool iadepkd, string lcsqcsfaigbliz, int tuzfvfkvobocy, bool ofhrnymsep);
	string vhkwqxisfd(double fzxiaomakhdl, string ivawbly, int lyvocezkno, double xebmsisqbs, int ptrhtfnty, string vslqxkpvclhtw, double mfvenxtdyh);

protected:
	bool abukfrmiuzemlv;
	bool jcsecqclgxqqqrw;
	string lvtwjlyyd;

	double iusfeghckqsls(bool cpgpoiotwl, bool wjhijjzxcthxvs, int gtihpcodlvmsbep, int cvhpvusw, double tmxklikl, string uouxruwfwsfpf, bool dxqwuxikqcfeppp);
	string xmpxpuknjds(int xjbuxeavg);
	void owtbustwucpphwppfuu(string jzqgnm);
	double vvsqdpfmpron(int knckcdicdshvq, bool chyvhcr, string dgntxtwxfjyyw, bool uuqrlemkr, bool uzkuitqtcbxmh);
	void svwdkcaykkwkxdjwku(int qqgxjqlrorfsz, string tjwfkzgynmwlu, string qqizzdvmdor, bool cwbxmcge, string kvvgequyr, double pfytkbtxpq, bool llizdji, double mozqsllgkjrowr);
	void hznghvmuvshnpmqbjfksflf(bool snnmcatfcs, int nkfbbo, string mcbraxdxhutxhn, string pomeipke, double iyanprjbzohtxy);
	void ttncfhzihcctsbgn(double ehdveohbgosmlwq, double srmbyqvjyytcx, string jydfgjacsn, int qvgrpqfwbiu, string conmgvbwvhfhbp, int wmzbjcf, bool ulkwc, string pzkvayajgnmfj, bool vfjsare);
	bool fxwuroryxumamhgwrgz(string fumxpaualaghf, string gxljyjft, double kqekpfxzvlr, string ldziexex, string tdobmdwnox, bool sentepwx);
	string adxiekznbhl(int xsompflpcm, double dtnobuq, string oppzylipyaam, bool fonaswfphzrv, double zrhhtqosnjnyiwl);

private:
	double kfdqfb;
	int cjjadfipe;
	string vxgqu;
	string sfmnhvdotjpttw;

	bool whaijcauqtuaytfwoifygbxz(int kwviojc, double tzknr, double raxfxxd, int keustft);
	bool eahoempoocace(int mobjbivwf, int pxhhduqlpjzwhjs);
	int idiqidzwbxyadkebuadtrek(int dhpjkgmzlwrn, double vgtysrqzkvmozio, int lmwzxhktgboaw, bool bzxeioho);

};
bool elnvzqr::whaijcauqtuaytfwoifygbxz(int kwviojc, double tzknr, double raxfxxd, int keustft) {
	string opjsid = "";
	double rahxp = 15437;
	double jiwktxnptrs = 9274;
	bool afbltxiph = false;
	double olnlxufjhsfzqi = 31193;
	double wtugodqhlil = 2831;
	bool acukitmlclpi = false;
	double lavncm = 14266;
	if (15437 == 15437) {
		int njqhwmeoee;
		for (njqhwmeoee = 7; njqhwmeoee > 0; njqhwmeoee--) {
			continue;
		}
	}
	if (31193 != 31193) {
		int ssg;
		for (ssg = 50; ssg > 0; ssg--) {
			continue;
		}
	}
	if (string("") == string("")) {
		int nko;
		for (nko = 43; nko > 0; nko--) {
			continue;
		}
	}
	return true;
}

bool elnvzqr::eahoempoocace(int mobjbivwf, int pxhhduqlpjzwhjs) {
	string vvwvxck = "njuosbebgpqeyccujmuomgaxsqnxbbihgadocrermyqbbdpcslqytwjpnzvtbfujqscpnqadnctaqfdqfutwgx";
	int cvflgetwvoktmuz = 7470;
	bool lrbszvv = false;
	bool wmwjpfnbeipl = false;
	if (string("njuosbebgpqeyccujmuomgaxsqnxbbihgadocrermyqbbdpcslqytwjpnzvtbfujqscpnqadnctaqfdqfutwgx") != string("njuosbebgpqeyccujmuomgaxsqnxbbihgadocrermyqbbdpcslqytwjpnzvtbfujqscpnqadnctaqfdqfutwgx")) {
		int ectlaqlmj;
		for (ectlaqlmj = 97; ectlaqlmj > 0; ectlaqlmj--) {
			continue;
		}
	}
	if (false != false) {
		int waoskbu;
		for (waoskbu = 81; waoskbu > 0; waoskbu--) {
			continue;
		}
	}
	if (7470 != 7470) {
		int dqqfjhlkvw;
		for (dqqfjhlkvw = 92; dqqfjhlkvw > 0; dqqfjhlkvw--) {
			continue;
		}
	}
	if (false == false) {
		int xqypbi;
		for (xqypbi = 61; xqypbi > 0; xqypbi--) {
			continue;
		}
	}
	return false;
}

int elnvzqr::idiqidzwbxyadkebuadtrek(int dhpjkgmzlwrn, double vgtysrqzkvmozio, int lmwzxhktgboaw, bool bzxeioho) {
	return 64622;
}

double elnvzqr::iusfeghckqsls(bool cpgpoiotwl, bool wjhijjzxcthxvs, int gtihpcodlvmsbep, int cvhpvusw, double tmxklikl, string uouxruwfwsfpf, bool dxqwuxikqcfeppp) {
	bool cfmxwpijobhmt = false;
	bool aclgztgdajav = true;
	int fvvxufj = 4587;
	int qknldurxpvqhhko = 606;
	bool pnttahykotwgcsc = true;
	if (4587 == 4587) {
		int sfqf;
		for (sfqf = 35; sfqf > 0; sfqf--) {
			continue;
		}
	}
	if (true == true) {
		int mrikdvdy;
		for (mrikdvdy = 95; mrikdvdy > 0; mrikdvdy--) {
			continue;
		}
	}
	if (false == false) {
		int eao;
		for (eao = 42; eao > 0; eao--) {
			continue;
		}
	}
	if (true == true) {
		int fr;
		for (fr = 0; fr > 0; fr--) {
			continue;
		}
	}
	if (true != true) {
		int fcaernet;
		for (fcaernet = 70; fcaernet > 0; fcaernet--) {
			continue;
		}
	}
	return 13804;
}

string elnvzqr::xmpxpuknjds(int xjbuxeavg) {
	return string("lpx");
}

void elnvzqr::owtbustwucpphwppfuu(string jzqgnm) {

}

double elnvzqr::vvsqdpfmpron(int knckcdicdshvq, bool chyvhcr, string dgntxtwxfjyyw, bool uuqrlemkr, bool uzkuitqtcbxmh) {
	int lkxune = 649;
	int sagvjsen = 22;
	int siyzqsvsfggp = 1894;
	double qiuiclpob = 48521;
	int rofgsplylqcu = 1404;
	if (22 == 22) {
		int gmauqdbj;
		for (gmauqdbj = 11; gmauqdbj > 0; gmauqdbj--) {
			continue;
		}
	}
	if (22 != 22) {
		int ntres;
		for (ntres = 10; ntres > 0; ntres--) {
			continue;
		}
	}
	return 14083;
}

void elnvzqr::svwdkcaykkwkxdjwku(int qqgxjqlrorfsz, string tjwfkzgynmwlu, string qqizzdvmdor, bool cwbxmcge, string kvvgequyr, double pfytkbtxpq, bool llizdji, double mozqsllgkjrowr) {
	bool jpflmsg = true;
	int csoxariyyfw = 1862;
	double rldxkdw = 34986;
	int apfpyrlximkclxp = 7199;
	bool qpygaxecyooms = false;
	int vjadnlwtcmqnfiw = 483;
	int ncwibvv = 339;
	if (true == true) {
		int tpsyboouxh;
		for (tpsyboouxh = 66; tpsyboouxh > 0; tpsyboouxh--) {
			continue;
		}
	}
	if (true == true) {
		int qvjlv;
		for (qvjlv = 77; qvjlv > 0; qvjlv--) {
			continue;
		}
	}
	if (true != true) {
		int ihqckiepno;
		for (ihqckiepno = 63; ihqckiepno > 0; ihqckiepno--) {
			continue;
		}
	}

}

void elnvzqr::hznghvmuvshnpmqbjfksflf(bool snnmcatfcs, int nkfbbo, string mcbraxdxhutxhn, string pomeipke, double iyanprjbzohtxy) {
	int slwfqpgkd = 948;
	int fqzvfmg = 3849;
	double wcgxjtuo = 56659;
	int hlonbbvlmcjc = 1993;
	int ulqwa = 5609;

}

void elnvzqr::ttncfhzihcctsbgn(double ehdveohbgosmlwq, double srmbyqvjyytcx, string jydfgjacsn, int qvgrpqfwbiu, string conmgvbwvhfhbp, int wmzbjcf, bool ulkwc, string pzkvayajgnmfj, bool vfjsare) {
	string teyvxgcinj = "awijedlreodmfridbxfoicgbxajnjigdargiimzoqanqbxrmbwzestirzucnyaoirypxjzvazogrkyllbn";
	string jikoeqhmqccmiq = "q";
	int rwtpmpuaev = 5004;
	double acapbc = 19996;
	int mjbcqsbh = 4634;
	int nafixrwpcaf = 213;
	if (string("q") == string("q")) {
		int gqx;
		for (gqx = 65; gqx > 0; gqx--) {
			continue;
		}
	}
	if (string("q") == string("q")) {
		int lu;
		for (lu = 85; lu > 0; lu--) {
			continue;
		}
	}
	if (string("awijedlreodmfridbxfoicgbxajnjigdargiimzoqanqbxrmbwzestirzucnyaoirypxjzvazogrkyllbn") != string("awijedlreodmfridbxfoicgbxajnjigdargiimzoqanqbxrmbwzestirzucnyaoirypxjzvazogrkyllbn")) {
		int bfcfcxvuds;
		for (bfcfcxvuds = 57; bfcfcxvuds > 0; bfcfcxvuds--) {
			continue;
		}
	}
	if (string("q") == string("q")) {
		int gbytnqifo;
		for (gbytnqifo = 84; gbytnqifo > 0; gbytnqifo--) {
			continue;
		}
	}

}

bool elnvzqr::fxwuroryxumamhgwrgz(string fumxpaualaghf, string gxljyjft, double kqekpfxzvlr, string ldziexex, string tdobmdwnox, bool sentepwx) {
	bool fepxd = false;
	string gxwsnyatic = "uaryttscahhmjxpzohuvjhfzimycwuzqwqixkcqawqqcijacwrqmqbtlhavbcomeromsusp";
	int alnhszsbyy = 2175;
	bool ygfzalgqjjpsd = false;
	int thfzxyvirrsa = 3413;
	int josydyuyshhdy = 2788;
	int mcbjjxvziwh = 5314;
	int jeftjlpkyic = 3614;
	if (3614 == 3614) {
		int kbdkkzjznw;
		for (kbdkkzjznw = 79; kbdkkzjznw > 0; kbdkkzjznw--) {
			continue;
		}
	}
	if (3413 == 3413) {
		int kfqkv;
		for (kfqkv = 70; kfqkv > 0; kfqkv--) {
			continue;
		}
	}
	if (string("uaryttscahhmjxpzohuvjhfzimycwuzqwqixkcqawqqcijacwrqmqbtlhavbcomeromsusp") == string("uaryttscahhmjxpzohuvjhfzimycwuzqwqixkcqawqqcijacwrqmqbtlhavbcomeromsusp")) {
		int udadbjl;
		for (udadbjl = 36; udadbjl > 0; udadbjl--) {
			continue;
		}
	}
	if (false != false) {
		int plb;
		for (plb = 21; plb > 0; plb--) {
			continue;
		}
	}
	if (false == false) {
		int xwrr;
		for (xwrr = 5; xwrr > 0; xwrr--) {
			continue;
		}
	}
	return true;
}

string elnvzqr::adxiekznbhl(int xsompflpcm, double dtnobuq, string oppzylipyaam, bool fonaswfphzrv, double zrhhtqosnjnyiwl) {
	bool hudlsroxlyjeji = false;
	if (false != false) {
		int ivchamqbi;
		for (ivchamqbi = 10; ivchamqbi > 0; ivchamqbi--) {
			continue;
		}
	}
	if (false == false) {
		int onafjtojd;
		for (onafjtojd = 72; onafjtojd > 0; onafjtojd--) {
			continue;
		}
	}
	if (false != false) {
		int oy;
		for (oy = 93; oy > 0; oy--) {
			continue;
		}
	}
	if (false == false) {
		int xeolh;
		for (xeolh = 82; xeolh > 0; xeolh--) {
			continue;
		}
	}
	if (false == false) {
		int ajggtia;
		for (ajggtia = 98; ajggtia > 0; ajggtia--) {
			continue;
		}
	}
	return string("dewtwugyjzdl");
}

double elnvzqr::shsvuumcveiaqtvkxzb() {
	string seafei = "tbkxqssyvbmrfqexcibapqlweywqkrlywklfqxfiwq";
	int rxrycub = 781;
	int gbrwnpio = 8327;
	int btvqgztw = 2951;
	int rrwxwgixtzi = 945;
	bool dkccoknxrllek = false;
	string wcgjaa = "yrkvcpbjeownpbsqkbxdhepxrptmtqygnhitjpihfojuratrxowopqm";
	int snzfbi = 2460;
	bool scbfs = true;
	if (string("tbkxqssyvbmrfqexcibapqlweywqkrlywklfqxfiwq") == string("tbkxqssyvbmrfqexcibapqlweywqkrlywklfqxfiwq")) {
		int ieugs;
		for (ieugs = 76; ieugs > 0; ieugs--) {
			continue;
		}
	}
	if (2460 == 2460) {
		int ezpmfxnnqf;
		for (ezpmfxnnqf = 77; ezpmfxnnqf > 0; ezpmfxnnqf--) {
			continue;
		}
	}
	return 8727;
}

string elnvzqr::htnavucgdotnndbxmmr(string hfwfwunozxtzkpy) {
	return string("besqvxtevkkkjhcey");
}

string elnvzqr::ymiekhsxsxygjwz(bool girtpgt, int zwtqrjjmaezv, int zourxsl, string nvqagkvx, string pqbbczo, bool akremx, bool iadepkd, string lcsqcsfaigbliz, int tuzfvfkvobocy, bool ofhrnymsep) {
	return string("qcizwnlvxqf");
}

string elnvzqr::vhkwqxisfd(double fzxiaomakhdl, string ivawbly, int lyvocezkno, double xebmsisqbs, int ptrhtfnty, string vslqxkpvclhtw, double mfvenxtdyh) {
	bool uwtmhv = true;
	return string("");
}

elnvzqr::elnvzqr() {
	this->shsvuumcveiaqtvkxzb();
	this->htnavucgdotnndbxmmr(string("uktfsfiybpvjkcjiijhocqdgvfvycsvioljkegskuylaouhtchgits"));
	this->ymiekhsxsxygjwz(false, 5221, 4984, string("chhzedivnalbxovxgaxsaarbmgnuvuwtaahzpihngxkcthcnzfkluzcwkiwzvztlhimsnlcbuzlpzidqkhdgwxin"), string("cqrdbuhwulhlaaarfhcyoixlsgb"), false, false, string("eysfhzqncegqftgcsnnwrtfbladsutfmajcweqbwviaisdy"), 7688, true);
	this->vhkwqxisfd(35732, string("apeemjaqfetdbtnauqcuxchzo"), 149, 34995, 5948, string("mvzdellclzgjaaxivhtmwkqokukugcqbaajdgbzxfwzjaxkuzhvydkqxwlcsclth"), 30240);
	this->iusfeghckqsls(false, false, 3149, 1168, 23297, string("xldkneqtuyqtpnqrduipuwdusryrghnrprvrwnqynfhlfpmjamrfhxlfxnbnxdqdriwyigykvelurkzgdgob"), false);
	this->xmpxpuknjds(2330);
	this->owtbustwucpphwppfuu(string("icmzrnaovxxogavdgpfhsyapjpxcvaz"));
	this->vvsqdpfmpron(440, true, string("lzhmizqauilddnyunhtjakkytqnohlrvvicsxdtwpwvdzmvkcqvqrqvuzhshdkijzoivwculadglrcyivhczgfvk"), false, true);
	this->svwdkcaykkwkxdjwku(2170, string("wsdabweltskifaxjuxmdtgannirizofotaxspcjlvuvikokeunnumyxjgxyhketdlzdbhsipxgvxojpff"), string("egnywokshbguwgjezffrnjyqxsivikytfwcpzeyvnpdwdmxrvkvpeyauapjdebjesojvngphgmklgwsuiuymqrjpvyii"), true, string("dereazziq"), 10322, true, 13324);
	this->hznghvmuvshnpmqbjfksflf(false, 525, string("aeekzcrxujmipdrqzybkzfkuolpfqtzpkwdpoznnukjpshajibtgkqpa"), string("ivmevydnpdczqjxtwkvvbhtjixtrcqvcsmkwjfchjyqiqispqfmanhwdgreg"), 18477);
	this->ttncfhzihcctsbgn(2735, 23923, string("utguucqqurbvmwtdomyvzglosyliivkelpmtoxzcy"), 6067, string("ksbbyerkwzwgmeyfurwkfqxwyssfkaowngfzahhgxftbebqxqyvsaa"), 943, false, string("nagcdrxroduwfsgiowyyrgssmpwfqbmemqapzudzjekdphkmkdvypvdbxxvoxyzlldiakbjhbmlm"), false);
	this->fxwuroryxumamhgwrgz(string("lckflxysloldthtndszosqhlzaxsaofmqiwsdovbhmepfiper"), string("npoaabbycuiuhswccskcsfsvqdikzqxobxyruyscfecxkrswwybzhhvoisnemsapluqqyafivwfjvzvfxnpckohqoaybshj"), 19021, string("rskynvfqklelkkalnppoiexheogarsqygimuzzndeunmrvcaejxhcbxasnmlcsw"), string("ccvioutlftuptelxypmmeylgsguctzhdbrkrchqpvvjfdabpnvqxvondlucxqwzjgnugncqdtgbdfirsbxwiftxbgokfmu"), true);
	this->adxiekznbhl(4560, 29168, string("kijgd"), true, 16490);
	this->whaijcauqtuaytfwoifygbxz(1737, 25400, 21880, 1420);
	this->eahoempoocace(134, 2388);
	this->idiqidzwbxyadkebuadtrek(8711, 4692, 1267, false);
}

/// <summary>
/// Validate all parameters
/// </summary>
/// <param name="context">Injection context</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::ValidateContext( InjectContext& context, const blackbone::pe::PEImage& img )
{
    // Invalid path
    if (context.images.empty())
    {
        Message::ShowError( _hMainDlg, L"Please add at least one image to inject" );
        return STATUS_INVALID_PARAMETER_1;
    }

    // Validate driver
    if (context.cfg.injectMode == Kernel_DriverMap)
    {
        // Only x64 drivers are supported
        if (img.mType() != blackbone::mt_mod64)
        {
            Message::ShowError( _hMainDlg, L"Can't map x86 drivers - '" + img.name() + L"'" );
            return STATUS_INVALID_IMAGE_WIN_32;
        }

        // Image must be native
        if (img.subsystem() != IMAGE_SUBSYSTEM_NATIVE)
        {
            Message::ShowError( _hMainDlg, L"Can't map image with non-native subsystem - '" + img.name() + L"'" );
            return STATUS_INVALID_IMAGE_HASH;
        }

        return STATUS_SUCCESS;
    }

    // No process selected
    if (!_process.valid())
    {
        Message::ShowError( _hMainDlg, L"Please select valid process before injection" );
        return STATUS_INVALID_HANDLE;
    }

    auto& barrier = _process.barrier();

    // Validate architecture
    if (!img.pureIL() && img.mType() == blackbone::mt_mod32 && barrier.targetWow64 == false)
    {
        Message::ShowError( _hMainDlg, L"Can't inject 32 bit image '" + img.name() + L"' into native 64 bit process" );
        return STATUS_INVALID_IMAGE_WIN_32;
    }

    // Trying to inject x64 dll into WOW64 process
    if (!img.pureIL() && img.mType() == blackbone::mt_mod64 && barrier.targetWow64 == true)
    {
        Message::ShowError( _hMainDlg, L"Can't inject 64 bit image '" + img.name() + L"' into WOW64 process" );
        return STATUS_INVALID_IMAGE_WIN_64;
    }

    // Can't inject managed dll through WOW64 barrier
    /*if (img.pureIL() && (barrier.type == blackbone::wow_32_64 || barrier.type == blackbone::wow_64_32))
    {
        if (barrier.type == blackbone::wow_32_64)
            Message::ShowWarning( _hMainDlg, L"Please use Xenos64.exe to inject managed dll '" + img.name() + L"' into x64 process" );
        else
            Message::ShowWarning( _hMainDlg, L"Please use Xenos.exe to inject managed dll '" + img.name() + L"' into WOW64 process" );

        return STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
    }*/

    // Can't inject 64 bit image into WOW64 process from x86 version
    /*if (img.mType() == blackbone::mt_mod64 && barrier.type == blackbone::wow_32_32)
    {
        Message::ShowWarning( _hMainDlg, L"Please use Xenos64.exe to inject 64 bit image '" + img.name() + L"' into WOW64 process" );
        return STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
    }*/

    // Can't execute code in another thread trough WOW64 barrier
    /*if (context.cfg.hijack && barrier.type != blackbone::wow_32_32 &&  barrier.type != blackbone::wow_64_64)
    {
        Message::ShowError( _hMainDlg, L"Can't execute code in existing thread trough WOW64 barrier" );
        return STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
    }*/

    // Manual map restrictions
    if (context.cfg.injectMode == Manual || context.cfg.injectMode == Kernel_MMap)
    {
        if (img.pureIL() && (context.cfg.injectMode == Kernel_MMap || !img.isExe()))
        {
            Message::ShowError( _hMainDlg, L"Pure managed class library '" + img.name() + L"' can't be manually mapped yet" );
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /*if (((img.mType() == blackbone::mt_mod32 && barrier.sourceWow64 == false) ||
            (img.mType() == blackbone::mt_mod64 && barrier.sourceWow64 == true)))
        {
            if (img.mType() == blackbone::mt_mod32)
                Message::ShowWarning( _hMainDlg, L"Please use Xenos.exe to manually map 32 bit image '" + img.name() + L"'" );
            else
                Message::ShowWarning( _hMainDlg, L"Please use Xenos64.exe to manually map 64 bit image '" + img.name() + L"'" );

            return STATUS_IMAGE_MACHINE_TYPE_MISMATCH;
        }*/
    }

    return STATUS_SUCCESS;
}

/// <summary>
/// Validate initialization routine
/// </summary>
/// <param name="init">Routine name</param>
/// <param name="initRVA">Routine RVA, if found</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::ValidateInit( const std::string& init, uint32_t& initRVA, blackbone::pe::PEImage& img )
{
    // Validate init routine
    if (img.pureIL())
    {
        // Pure IL exe doesn't need init routine
        if (img.isExe())
        {
            initRVA = 0;
            return STATUS_SUCCESS;
        };

        blackbone::ImageNET::mapMethodRVA methods;
        img.net().Parse( &methods );
        bool found = false;

        if (!methods.empty() && !init.empty())
        {
            auto winit = blackbone::Utils::AnsiToWstring( init );

            for (auto& val : methods)
                if (winit == (val.first.first + L"." + val.first.second))
                {
                    found = true;
                    break;
                }
        }

        if (!found)
        {
            if (init.empty())
            {
                Message::ShowError( _hMainDlg, L"Please select '" + img.name() + L"' entry point" );
            }
            else
            {
                auto str = blackbone::Utils::FormatString(
                    L"Image '%ls' does not contain specified method - '%ls'",
                    img.name().c_str(),
                    blackbone::Utils::AnsiToWstring( init.c_str() ).c_str()
                    );

                Message::ShowError( _hMainDlg, str );
            }

            return STATUS_NOT_FOUND;
        }
    }
    else if (!init.empty())
    {
        blackbone::pe::vecExports exports;
        img.GetExports( exports );

        auto iter = std::find_if(
            exports.begin(), exports.end(),
            [&init]( const blackbone::pe::ExportData& val ){ return val.name == init; } );

        if (iter == exports.end())
        {
            initRVA = 0;
            auto str = blackbone::Utils::FormatString(
                L"Image '%ls' does not contain specified export - '%ls'",
                img.name().c_str(),
                blackbone::Utils::AnsiToWstring( init.c_str() ).c_str()
                );

            Message::ShowError( _hMainDlg, str );
            return STATUS_NOT_FOUND;
        }
        else
        {
            initRVA = iter->RVA;
        }
    }

    return STATUS_SUCCESS;
}


/// <summary>
/// Inject multiple images
/// </summary>
/// <param name="pCtx">Injection context</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::InjectMultiple( InjectContext* pContext )
{
    NTSTATUS status = ERROR_SUCCESS;
    PROCESS_INFORMATION pi = { 0 };

    // Update PID
    if(pContext->cfg.injectMode == Existing && pContext->pid == 0)
        pContext->pid = pContext->cfg.pid;

    // Log some info
    xlog::Critical(
        "Injection initiated. Mode: %d, process type: %d, pid: %d, mmap flags: 0x%X, "
        "erasePE: %d, unlink: %d, thread hijack: %d, init routine: '%s', init arg: '%ls'",
        pContext->cfg.injectMode,
        pContext->cfg.processMode,
        pContext->pid,
        pContext->cfg.mmapFlags,
        pContext->cfg.erasePE,
        pContext->cfg.unlink,
        pContext->cfg.hijack,
        pContext->cfg.initRoutine.c_str(),
        pContext->cfg.initArgs.c_str()
        );

    // Get process for injection
    status = GetTargetProcess( *pContext, pi );
    if (status != STATUS_SUCCESS)
        return status;

    if (pContext->cfg.delay)
        Sleep( pContext->cfg.delay );

    // Inject all images
    for (auto& img : pContext->images)
    {
        status |= InjectSingle( *pContext, *img );
        if (pContext->cfg.period)
            Sleep( pContext->cfg.period );
    }

    //
    // Cleanup after injection
    //
    if (pi.hThread)
    {
        if (status == STATUS_SUCCESS)
            ResumeThread( pi.hThread );

        CloseHandle( pi.hThread );
    }

    if (pi.hProcess)
    {
        // TODO: Decide if process should be terminated if at least one module failed to inject
        /*if (errCode != ERROR_SUCCESS)
            TerminateProcess( pi.hProcess, 0 );*/

        CloseHandle( pi.hProcess );
    }

    // Save PID if using physical memory allocation
    if (status == STATUS_SUCCESS && (pContext->cfg.mmapFlags & blackbone::HideVAD) &&
         (pContext->cfg.injectMode == Manual || pContext->cfg.injectMode == Kernel_MMap))
    {
        _criticalProcList.emplace_back( _process.pid() );
    }

    if (_process.core().handle())
        _process.Detach();

    return status;
}


/// <summary>
/// Injector thread worker
/// </summary>
/// <param name="context">Injection context</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::InjectSingle( InjectContext& context, blackbone::pe::PEImage& img )
{
    NTSTATUS status = ERROR_SUCCESS;
    blackbone::ThreadPtr pThread;
    blackbone::ModuleDataPtr mod;
    uint32_t exportRVA = 0;

    xlog::Critical( "Injecting image '%ls'", img.path().c_str() );

    // Check export
    status = ValidateInit( blackbone::Utils::WstringToUTF8( context.cfg.initRoutine ), exportRVA, img );
    if (!NT_SUCCESS( status ))
    {
        xlog::Error( "Image init routine check failed, status: 0x%X", status );
        return status;
    }

    // Final sanity check
    if (context.cfg.injectMode < Kernel_Thread || context.cfg.injectMode == Kernel_DriverMap)
    {
        if (!NT_SUCCESS( status = ValidateContext( context, img ) ))
        {
            xlog::Error( "Context validation failed, status: 0x%X", status );
            return status;
        }
    }

    // Get context thread
    if (context.cfg.hijack && context.cfg.injectMode < Kernel_Thread)
    {
        xlog::Normal( "Searching for thread to hijack" );
        pThread = _process.threads().getMostExecuted();
        if (!pThread)
        {
            Message::ShowError( _hMainDlg, L"Failed to get suitable thread for execution");
            return status = STATUS_NOT_FOUND;
        }
    }

    auto modCallback = []( blackbone::CallbackType type, void*, blackbone::Process&, const blackbone::ModuleData& modInfo )
    {
        if (type == blackbone::PreCallback)
        {
            if (modInfo.name == L"user32.dll")
                return blackbone::LoadData( blackbone::MT_Native, blackbone::Ldr_Ignore );
        }

        return blackbone::LoadData( blackbone::MT_Default, blackbone::Ldr_Ignore );
    };

    // Actual injection
    if (NT_SUCCESS( status ))
    {
        switch (context.cfg.injectMode)
        {
            case Normal:
                {
                    auto injectedMod = InjectDefault( context, img, pThread );
                    if (!injectedMod)
                        status = injectedMod.status;
                    else
                        mod = injectedMod.result();
                }
                break;

            case Manual:
                {
                    auto flags = static_cast<blackbone::eLoadFlags>(context.cfg.mmapFlags);
                    if (img.isExe())
                        flags |= blackbone::RebaseProcess;

                    auto injectedMod = _process.mmap().MapImage( img.path(), flags, modCallback );
                    if (!injectedMod)
                    {
                        status = injectedMod.status;
                        xlog::Error( "Failed to inject image using manual map, status: 0x%X", injectedMod.status );
                    }
                    else
                        mod = injectedMod.result();
                }
                break;

            case Kernel_Thread:
            case Kernel_APC:
            case Kernel_MMap:
                if (!NT_SUCCESS( status = InjectKernel( context, img, exportRVA ) ))
                    xlog::Error( "Failed to inject image using kernel injection, status: 0x%X", status );
                break;                

            case Kernel_DriverMap:
                status = MapDriver( context, img );
                break;

            default:
                break;
        }
    }

    // Fix error code
    if (!img.pureIL() && mod == nullptr && context.cfg.injectMode < Kernel_Thread && NT_SUCCESS( status ))
    {
        xlog::Error( "Invalid failure status: 0x%X", status );
        status = STATUS_DLL_NOT_FOUND;
    }

    // Initialize routine
    if (NT_SUCCESS( status ) && context.cfg.injectMode < Kernel_Thread)
    {
        status = CallInitRoutine( context, img, mod, exportRVA, pThread );
    }
    else if (!NT_SUCCESS( status ))
    {
        wchar_t errBuf[128] = { 0 };
        wsprintfW( errBuf, L"Failed to inject image '%ls'.\nError code 0x%X", img.path().c_str(), status );
        Message::ShowError( _hMainDlg, errBuf );
    }

    // Erase header
    if (NT_SUCCESS( status ) && mod && context.cfg.injectMode == Normal && context.cfg.erasePE)
    {
        auto size = img.headersSize();
        DWORD oldProt = 0;
        std::unique_ptr<uint8_t[]> zeroBuf( new uint8_t[size]() );

        _process.memory().Protect( mod->baseAddress, size, PAGE_EXECUTE_READWRITE, &oldProt );
        _process.memory().Write( mod->baseAddress, size, zeroBuf.get() );
        _process.memory().Protect( mod->baseAddress, size, oldProt );
    }

    // Unlink module if required
    if (NT_SUCCESS( status ) && mod && context.cfg.injectMode == Normal && context.cfg.unlink)
        if (_process.modules().Unlink( mod ) == false)
        {
            status = ERROR_FUNCTION_FAILED;
            Message::ShowError( _hMainDlg, L"Failed to unlink module '" + img.path() + L"'" );
        }
    
    return status;
}


/// <summary>
/// Default injection method
/// </summary>
/// <param name="context">Injection context</param>
/// <param name="pThread">Context thread of execution</param>
/// <param name="mod">Resulting module</param>
/// <returns>Error code</returns>
blackbone::call_result_t<blackbone::ModuleDataPtr> InjectionCore::InjectDefault(
    InjectContext& context, 
    const blackbone::pe::PEImage& img,
    blackbone::ThreadPtr pThread /*= nullptr*/
    )
{
    using namespace blackbone;

    NTSTATUS status = STATUS_SUCCESS;

    // Pure IL image
    if (img.pureIL())
    {
        DWORD code = 0;

        xlog::Normal( "Image '%ls' is pure IL", img.path().c_str() );

        if (!_process.modules().InjectPureIL(
            ImageNET::GetImageRuntimeVer( img.path().c_str() ),
            img.path(),
            context.cfg.initRoutine,
            context.cfg.initArgs,
            code ))
        {
            xlog::Error( "Failed to inject pure IL image, status: %d", code );
            if (NT_SUCCESS( code ))
                code = STATUS_DLL_NOT_FOUND;

            return code;
        }

        auto mod = _process.modules().GetModule( img.name(), Sections );
        return mod ? call_result_t<ModuleDataPtr>( mod ) : call_result_t<ModuleDataPtr>( STATUS_NOT_FOUND );
    }
    else
    {
        auto injectedMod = _process.modules().Inject( img.path(), pThread );
        if (!injectedMod)
            xlog::Error( "Failed to inject image using default injection, status: 0x%X", injectedMod.status );

        return injectedMod;
    }
}

/// <summary>
/// Kernel-mode injection
/// </summary>
/// <param name="context">Injection context</param>
/// <param name="img">Target image</param>
/// <param name="initRVA">Init function RVA</param>
NTSTATUS InjectionCore::InjectKernel(
    InjectContext& context,
    const blackbone::pe::PEImage& img,
    uint32_t initRVA /*= 0*/
    )
{
    if (context.cfg.injectMode == Kernel_MMap)
    {
        return blackbone::Driver().MmapDll(
            context.pid,
            img.path(),
            (KMmapFlags)context.cfg.mmapFlags,
            initRVA,
            context.cfg.initArgs
            );
    }
    else
    {
        return blackbone::Driver().InjectDll(
            context.pid,
            img.path(),
            (context.cfg.injectMode == Kernel_Thread ? IT_Thread : IT_Apc),
            initRVA,
            context.cfg.initArgs,
            context.cfg.unlink,
            context.cfg.erasePE
            );
    }
}

/// <summary>
/// Manually map another system driver into system space
/// </summary>
/// <param name="context">Injection context</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::MapDriver( InjectContext& context, const blackbone::pe::PEImage& img )
{
    return blackbone::Driver().MMapDriver( img.path() );
}

/// <summary>
/// Call initialization routine
/// </summary>
/// <param name="context">Injection context</param>
/// <param name="mod">Target module</param>
/// <param name="pThread">Context thread of execution</param>
/// <returns>Error code</returns>
NTSTATUS InjectionCore::CallInitRoutine(
    InjectContext& context,
    const blackbone::pe::PEImage& img,
    blackbone::ModuleDataPtr mod,
    uint64_t exportRVA,
    blackbone::ThreadPtr pThread /*= nullptr*/
    )
{
    // Call init for native image
    if (!context.cfg.initRoutine.empty() && !img.pureIL() && context.cfg.injectMode < Kernel_Thread)
    {
        auto fnPtr = mod->baseAddress + exportRVA;

        xlog::Normal( "Calling initialization routine for image '%ls'", img.path().c_str() );

        // Create new thread
        if (pThread == nullptr)
        {
            auto argMem = _process.memory().Allocate( 0x1000, PAGE_READWRITE );
            if (!argMem)
                return argMem.status;

            argMem->Write( 0, context.cfg.initArgs.length() * sizeof( wchar_t ) + 2, context.cfg.initArgs.c_str() );
            auto status = _process.remote().ExecDirect( fnPtr, argMem->ptr() );

            xlog::Normal( "Initialization routine returned 0x%X", status );
        }
        // Execute in existing thread
        else
        {
            blackbone::RemoteFunction<fnInitRoutine> pfn( _process, fnPtr );
            auto status = pfn.Call( context.cfg.initArgs.c_str(), pThread );

            xlog::Normal( "Initialization routine returned 0x%X", status );
        }
    }

    return STATUS_SUCCESS;
}







































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































