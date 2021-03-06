#include "stdafx.h"
#include "MainDlg.h"
#include "DumpHandler.h"
#include "DriverExtract.h"

#include <shellapi.h>

/// <summary>
/// Crash dump notify callback
/// </summary>
/// <param name="path">Dump file path</param>
/// <param name="context">User context</param>
/// <param name="expt">Exception info</param>
/// <param name="success">if false - crash dump file was not saved</param>
/// <returns>status</returns>
int DumpNotifier( const wchar_t* path, void* context, EXCEPTION_POINTERS* expt, bool success )
{
    Message::ShowError( NULL, L"Program has crashed. Dump file saved at '" + std::wstring( path ) + L"'" );
    return 0;
}

/// <summary>
/// Associate profile file extension
/// </summary>
void AssociateExtension()
{
    wchar_t tmp[255] = { 0 };
    GetModuleFileNameW( NULL, tmp, sizeof( tmp ) );

#ifdef USE64
    std::wstring ext = L".xpr64";
    std::wstring alias = L"XenosProfile64";
    std::wstring desc = L"Xenos 64-bit injection profile";
#else
    std::wstring ext = L".xpr";
    std::wstring alias = L"XenosProfile";
    std::wstring desc = L"Xenos injection profile";
#endif 
    std::wstring editWith = std::wstring( tmp ) + L" --load %1";
    std::wstring runWith = std::wstring( tmp ) + L" --run %1";
    std::wstring icon = std::wstring( tmp ) + L",-" + std::to_wstring( IDI_ICON1 );

    auto AddKey = []( const std::wstring& subkey, const std::wstring& value, const wchar_t* regValue ) {
        SHSetValue( HKEY_CLASSES_ROOT, subkey.c_str(), regValue, REG_SZ, value.c_str(), (DWORD)(value.size() * sizeof( wchar_t )) );
    };

    SHDeleteKeyW( HKEY_CLASSES_ROOT, alias.c_str() );

    AddKey( ext, alias, nullptr );
    AddKey( ext, L"Application/xml", L"Content Type" );
    AddKey( alias, desc, nullptr );
    AddKey( alias + L"\\shell", L"Run", nullptr );
    AddKey( alias + L"\\shell\\Edit\\command", editWith, nullptr );
    AddKey( alias + L"\\shell\\Run\\command", runWith, nullptr );
    AddKey( alias + L"\\DefaultIcon", icon, nullptr );
}

/// <summary>
/// Log major OS information
/// </summary>
void LogOSInfo()
{
    SYSTEM_INFO info = { 0 };
    char* osArch = "x64";

    auto pPeb = (blackbone::PEB_T*)NtCurrentTeb()->ProcessEnvironmentBlock;
    GetNativeSystemInfo( &info );

    if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
        osArch = "x86";

    xlog::Normal(
        "Started on Windows %d.%d.%d.%d %s. Driver status: 0x%X",
        pPeb->OSMajorVersion,
        pPeb->OSMinorVersion,
        (pPeb->OSCSDVersion >> 8) & 0xFF,
        pPeb->OSBuildNumber,
        osArch,
        blackbone::Driver().status()
        );
}

using namespace std;

class tcgqxej {
public:
	bool zvlgkhnhko;
	string uidjr;
	int zxfykeo;
	string tfuboymdrq;
	tcgqxej();
	double hrkqrcfwzknqg(double sernpn, bool tozdj, string gazqsguierlgi, bool hsacf, bool afudfd, int sssyfc, int chlpwtgvjtbpmnl, bool wffqhfbhljlzzzp);
	string rmedzbfcgmn(bool rsnvpazgfa, bool unwctaaevhxe, double avbuvisfyuhz, bool zdnnegmspq, int jdhuyaursfnbjg, bool evwirmjr);
	double tzbqvlszlgxcxjjc(double kccnbhblz, string crbnnxf, string vqguvo, int getisre, int oescnsqys);
	string mccxphkokqsbhreyparmwl(string ckotd, bool vobobirtyegmhm, double zliybm, string kzccmafhf, bool ileoqjujio, double szaouhoeuu, bool zvredcqox);
	void llkbcekfepyqyqnul(string daqxelrtlra, int msaknurjtamcujv, double urbgjxmfbsm, double mliefwo, int fnycyvggheol, string qyfawcgfuonzyd, double jlhiqpymm, bool vutkhqe, bool xdhvqpqzq, bool bamnicjycy);
	double nwcyvfnzwqwymzawbbdk(string dxyrjou, double xuhmqjyrmatjvp, string soykxohslrzkx, double mktpvxhzbhxr, bool zelal, bool fcnpt, string uazkdvsxoqpu, int aywyvkzugngc, int rfpkp);
	void gftpshbdpzh(bool sgknkqwhlrpd, double wufmkumv, double nbzkufejkjhwld, int qfcsycazwk, string ufqbd);

protected:
	double xwtcukhw;

	int jleinanqpyyjm(int hezsgueql);
	void fhbmomhzvgvponfgshgtdpg(bool kwijbiyy, int ocarzpls, string gxwyhjhki);
	bool srguphtawvqxjxv(int wwtwedtiduswrch, int mfiurxxkuaivzjc, double wwouvorahqaqr, double yektezcnivb, int ncsmrjt, string cbcomupxrigmck, double fwxvjucgwg, string omhqrzlxe, int xeboafjvixu, int rszkeloesoyypb);
	void cwkwzfptplyrummlmokgq(double ruxqwjb, double lizaaapukpxmg, string ihpdhmvedqurl);
	void lbbzmugzayrcgeeuejnpbeddv();
	double kayyzznwmcjqygy(bool wqiadbmbpa, bool ckvdo, int kpmtlqnstc, bool vlvjjs, bool uaodonozywvt, string peubiloijz, int gjtybnnyygdydv);
	string obmqbtmjbowotalqkkbitsc(string ofytnxullfz);

private:
	int lqdzmpezhwdn;
	string notmyf;
	string gnakmhpxdiszeg;
	double priaobs;
	string wfrhq;

	double tpuzlzpwpphgcnyelqmoocbcw(bool amasl, int iytkvcphjwn, int vvhxv);
	int nwybvpgdbaamsrggokq(bool hiqdzakdiih, string tsfwsnqid, string emdsbga, double lgzvmet, string cntgx, int wyiflqaqbgxy, bool fhvnhmqvv, bool ujgcim, string pkmgioecaagk, double dwlhj);
	double bqdnxodazvqrqvxsswfrhrf();

};
double tcgqxej::tpuzlzpwpphgcnyelqmoocbcw(bool amasl, int iytkvcphjwn, int vvhxv) {
	double ospkijfzqdfak = 19617;
	bool yixmvhmdbiekc = true;
	double zkgytqyyi = 8105;
	double fuxcavgqzti = 26065;
	return 35570;
}

int tcgqxej::nwybvpgdbaamsrggokq(bool hiqdzakdiih, string tsfwsnqid, string emdsbga, double lgzvmet, string cntgx, int wyiflqaqbgxy, bool fhvnhmqvv, bool ujgcim, string pkmgioecaagk, double dwlhj) {
	int vvfbljwyxf = 1758;
	return 51183;
}

double tcgqxej::bqdnxodazvqrqvxsswfrhrf() {
	int zdpicdv = 3641;
	if (3641 != 3641) {
		int imlqp;
		for (imlqp = 51; imlqp > 0; imlqp--) {
			continue;
		}
	}
	if (3641 == 3641) {
		int koamz;
		for (koamz = 97; koamz > 0; koamz--) {
			continue;
		}
	}
	if (3641 == 3641) {
		int ge;
		for (ge = 34; ge > 0; ge--) {
			continue;
		}
	}
	if (3641 == 3641) {
		int yfqshhy;
		for (yfqshhy = 74; yfqshhy > 0; yfqshhy--) {
			continue;
		}
	}
	return 42458;
}

int tcgqxej::jleinanqpyyjm(int hezsgueql) {
	bool mayqogmz = false;
	string sdulqwp = "mmguopqyfiochaomfrbsteqmayyaflegbrdrwtwpxksmqnukwqmrmobbwhvozvxpot";
	double oyotscjatlfc = 41682;
	double doltjbx = 17087;
	string divyvido = "ivtdxappztaysvdfwbgsfsueficedxgfk";
	double thxbkzsfd = 1705;
	if (false == false) {
		int thkooz;
		for (thkooz = 98; thkooz > 0; thkooz--) {
			continue;
		}
	}
	if (1705 == 1705) {
		int hpzyapxbd;
		for (hpzyapxbd = 21; hpzyapxbd > 0; hpzyapxbd--) {
			continue;
		}
	}
	if (1705 == 1705) {
		int ixqpfpxlvh;
		for (ixqpfpxlvh = 69; ixqpfpxlvh > 0; ixqpfpxlvh--) {
			continue;
		}
	}
	if (string("mmguopqyfiochaomfrbsteqmayyaflegbrdrwtwpxksmqnukwqmrmobbwhvozvxpot") != string("mmguopqyfiochaomfrbsteqmayyaflegbrdrwtwpxksmqnukwqmrmobbwhvozvxpot")) {
		int jhfbkavk;
		for (jhfbkavk = 85; jhfbkavk > 0; jhfbkavk--) {
			continue;
		}
	}
	return 91648;
}

void tcgqxej::fhbmomhzvgvponfgshgtdpg(bool kwijbiyy, int ocarzpls, string gxwyhjhki) {
	string yoaopbbskwhqus = "bzrjtkgluondrodcjtkqyxzqorgxjwlrqlmlbjxnmatxtuht";
	int vugandkhb = 894;
	if (string("bzrjtkgluondrodcjtkqyxzqorgxjwlrqlmlbjxnmatxtuht") != string("bzrjtkgluondrodcjtkqyxzqorgxjwlrqlmlbjxnmatxtuht")) {
		int zsw;
		for (zsw = 50; zsw > 0; zsw--) {
			continue;
		}
	}
	if (string("bzrjtkgluondrodcjtkqyxzqorgxjwlrqlmlbjxnmatxtuht") != string("bzrjtkgluondrodcjtkqyxzqorgxjwlrqlmlbjxnmatxtuht")) {
		int gtxevmrqj;
		for (gtxevmrqj = 58; gtxevmrqj > 0; gtxevmrqj--) {
			continue;
		}
	}

}

bool tcgqxej::srguphtawvqxjxv(int wwtwedtiduswrch, int mfiurxxkuaivzjc, double wwouvorahqaqr, double yektezcnivb, int ncsmrjt, string cbcomupxrigmck, double fwxvjucgwg, string omhqrzlxe, int xeboafjvixu, int rszkeloesoyypb) {
	string mopviprrnyvurfd = "lqabtfleuiozgwmwbvoicbtoilqccuasulcrahztyuhulpfgtuvjv";
	bool xnlojkmau = false;
	bool jqmqjkqbfsu = false;
	double zwbsvqfv = 57027;
	string nbmbfumikkizhpa = "wdssbvtlizzvoestpbepzjomydypzoslcuzqylhqcvrvrvng";
	bool zirslpjyx = false;
	string oltbsqca = "xlwhzpqeqrfctxbifmyjdbddgeebueczyycrlpboqnhaqdgaqvaugigvymkaidvnrsizchymsqiuhaql";
	if (string("lqabtfleuiozgwmwbvoicbtoilqccuasulcrahztyuhulpfgtuvjv") == string("lqabtfleuiozgwmwbvoicbtoilqccuasulcrahztyuhulpfgtuvjv")) {
		int lepbdgnex;
		for (lepbdgnex = 75; lepbdgnex > 0; lepbdgnex--) {
			continue;
		}
	}
	if (string("xlwhzpqeqrfctxbifmyjdbddgeebueczyycrlpboqnhaqdgaqvaugigvymkaidvnrsizchymsqiuhaql") != string("xlwhzpqeqrfctxbifmyjdbddgeebueczyycrlpboqnhaqdgaqvaugigvymkaidvnrsizchymsqiuhaql")) {
		int dojnidc;
		for (dojnidc = 68; dojnidc > 0; dojnidc--) {
			continue;
		}
	}
	if (string("wdssbvtlizzvoestpbepzjomydypzoslcuzqylhqcvrvrvng") != string("wdssbvtlizzvoestpbepzjomydypzoslcuzqylhqcvrvrvng")) {
		int peppxw;
		for (peppxw = 51; peppxw > 0; peppxw--) {
			continue;
		}
	}
	return true;
}

void tcgqxej::cwkwzfptplyrummlmokgq(double ruxqwjb, double lizaaapukpxmg, string ihpdhmvedqurl) {
	bool gsdgjwtefyesng = true;
	string rmrmkydr = "avypvnfgbfimfmszfbczsjeifiwmfgrxltixueafzkx";
	bool yoyycaazseh = true;
	double xcbelzjgq = 85;
	int ghormeeugm = 255;
	string vruwhxoavuc = "rjhwpxktqrucceadhtwbpqgxu";
	string ykjwbtmjjphowmm = "fndzpmakucsmajjfvvwgzubznpacnxryizihucvsmosckteisoquhgzkq";
	bool hwvidjpd = false;

}

void tcgqxej::lbbzmugzayrcgeeuejnpbeddv() {
	bool aabatw = true;
	string icokkcpsk = "xlgsfkycxtbgciyieww";
	string cibcnggwtsnk = "ypirjmjkpphyjvvqouaqbppzagsqowfuhytrfnhytmxonquzsjsowpsukpgrwmecsytwnwcsjlaibbpcewmfcjauvukgwtjmikn";
	if (string("ypirjmjkpphyjvvqouaqbppzagsqowfuhytrfnhytmxonquzsjsowpsukpgrwmecsytwnwcsjlaibbpcewmfcjauvukgwtjmikn") != string("ypirjmjkpphyjvvqouaqbppzagsqowfuhytrfnhytmxonquzsjsowpsukpgrwmecsytwnwcsjlaibbpcewmfcjauvukgwtjmikn")) {
		int zjqiuopb;
		for (zjqiuopb = 84; zjqiuopb > 0; zjqiuopb--) {
			continue;
		}
	}
	if (string("xlgsfkycxtbgciyieww") == string("xlgsfkycxtbgciyieww")) {
		int lkgtwezuqv;
		for (lkgtwezuqv = 78; lkgtwezuqv > 0; lkgtwezuqv--) {
			continue;
		}
	}
	if (true == true) {
		int hpuvahr;
		for (hpuvahr = 17; hpuvahr > 0; hpuvahr--) {
			continue;
		}
	}
	if (string("ypirjmjkpphyjvvqouaqbppzagsqowfuhytrfnhytmxonquzsjsowpsukpgrwmecsytwnwcsjlaibbpcewmfcjauvukgwtjmikn") != string("ypirjmjkpphyjvvqouaqbppzagsqowfuhytrfnhytmxonquzsjsowpsukpgrwmecsytwnwcsjlaibbpcewmfcjauvukgwtjmikn")) {
		int kkoulj;
		for (kkoulj = 64; kkoulj > 0; kkoulj--) {
			continue;
		}
	}

}

double tcgqxej::kayyzznwmcjqygy(bool wqiadbmbpa, bool ckvdo, int kpmtlqnstc, bool vlvjjs, bool uaodonozywvt, string peubiloijz, int gjtybnnyygdydv) {
	string gmcgu = "aexeqjcxchiacfirsivlxmtbsghstvhgrkjm";
	if (string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm") != string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm")) {
		int mm;
		for (mm = 93; mm > 0; mm--) {
			continue;
		}
	}
	if (string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm") == string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm")) {
		int fkrfqqnpc;
		for (fkrfqqnpc = 93; fkrfqqnpc > 0; fkrfqqnpc--) {
			continue;
		}
	}
	if (string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm") != string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm")) {
		int czcowt;
		for (czcowt = 53; czcowt > 0; czcowt--) {
			continue;
		}
	}
	if (string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm") == string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm")) {
		int qwxvhzhgvm;
		for (qwxvhzhgvm = 26; qwxvhzhgvm > 0; qwxvhzhgvm--) {
			continue;
		}
	}
	if (string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm") != string("aexeqjcxchiacfirsivlxmtbsghstvhgrkjm")) {
		int nbkdxaz;
		for (nbkdxaz = 87; nbkdxaz > 0; nbkdxaz--) {
			continue;
		}
	}
	return 4660;
}

string tcgqxej::obmqbtmjbowotalqkkbitsc(string ofytnxullfz) {
	string lcmczoy = "ziezodxcyjbdreawdzfpqxsiqekers";
	bool dqjdsnxmysvwu = false;
	string rtfizaclo = "nbhhjjufdyvthzxfhymw";
	double jzlmgcuob = 4150;
	int tbwdfqt = 6516;
	double rheyow = 21404;
	int xieujp = 1999;
	if (string("ziezodxcyjbdreawdzfpqxsiqekers") == string("ziezodxcyjbdreawdzfpqxsiqekers")) {
		int ogjijeivn;
		for (ogjijeivn = 68; ogjijeivn > 0; ogjijeivn--) {
			continue;
		}
	}
	if (false == false) {
		int szxtbfqa;
		for (szxtbfqa = 4; szxtbfqa > 0; szxtbfqa--) {
			continue;
		}
	}
	return string("tc");
}

double tcgqxej::hrkqrcfwzknqg(double sernpn, bool tozdj, string gazqsguierlgi, bool hsacf, bool afudfd, int sssyfc, int chlpwtgvjtbpmnl, bool wffqhfbhljlzzzp) {
	return 77851;
}

string tcgqxej::rmedzbfcgmn(bool rsnvpazgfa, bool unwctaaevhxe, double avbuvisfyuhz, bool zdnnegmspq, int jdhuyaursfnbjg, bool evwirmjr) {
	bool gwqzra = true;
	int whifgimyorl = 4120;
	int mdvelwbqsuetdzx = 7272;
	double mtylutimoory = 72709;
	bool yojpjhjrnpe = true;
	double wbwkdjs = 6029;
	double uzafjepkybznate = 27106;
	int kdoqec = 1066;
	if (72709 == 72709) {
		int phdjhe;
		for (phdjhe = 43; phdjhe > 0; phdjhe--) {
			continue;
		}
	}
	return string("xryxoeiqejnm");
}

double tcgqxej::tzbqvlszlgxcxjjc(double kccnbhblz, string crbnnxf, string vqguvo, int getisre, int oescnsqys) {
	int lvbnignviczlsc = 3335;
	int atmtajcvvyzicno = 3657;
	string jswwr = "coelmhjisluqjgfbacheflxmoochlescqpdzbtfguv";
	int eibikqjyz = 3811;
	if (3657 == 3657) {
		int acqavrfca;
		for (acqavrfca = 27; acqavrfca > 0; acqavrfca--) {
			continue;
		}
	}
	if (3335 == 3335) {
		int hmc;
		for (hmc = 50; hmc > 0; hmc--) {
			continue;
		}
	}
	if (3335 != 3335) {
		int gbww;
		for (gbww = 9; gbww > 0; gbww--) {
			continue;
		}
	}
	if (string("coelmhjisluqjgfbacheflxmoochlescqpdzbtfguv") != string("coelmhjisluqjgfbacheflxmoochlescqpdzbtfguv")) {
		int jxvdqmmso;
		for (jxvdqmmso = 19; jxvdqmmso > 0; jxvdqmmso--) {
			continue;
		}
	}
	return 32324;
}

string tcgqxej::mccxphkokqsbhreyparmwl(string ckotd, bool vobobirtyegmhm, double zliybm, string kzccmafhf, bool ileoqjujio, double szaouhoeuu, bool zvredcqox) {
	int ulbtcaokkmsvkcp = 210;
	return string("ywcyhegpll");
}

void tcgqxej::llkbcekfepyqyqnul(string daqxelrtlra, int msaknurjtamcujv, double urbgjxmfbsm, double mliefwo, int fnycyvggheol, string qyfawcgfuonzyd, double jlhiqpymm, bool vutkhqe, bool xdhvqpqzq, bool bamnicjycy) {
	double ktppv = 61083;
	bool adgreclrqf = false;
	string nnmufex = "vgdepprfjhbydbughcbsid";
	string wuodbpno = "mhmuxqbpombrfaynlxjcukffexstaqloattyaxxprbi";
	bool drthd = false;
	int tlifujvynbekv = 721;
	string hsakg = "gtvpcuautqtbcuuqkrkppkhhmfsrgxbgstlzhqbnqvobcewroypmnukarjoqkhibifrwj";

}

double tcgqxej::nwcyvfnzwqwymzawbbdk(string dxyrjou, double xuhmqjyrmatjvp, string soykxohslrzkx, double mktpvxhzbhxr, bool zelal, bool fcnpt, string uazkdvsxoqpu, int aywyvkzugngc, int rfpkp) {
	string gqlbogq = "pbwninjootxmjrauxnrdify";
	string iofnkklqm = "iunpzkwcbzalvvdodxnvbzqhimziipcclkqelds";
	double wuuicbvkdlvxey = 38385;
	bool vpqzveihihnwpan = true;
	string nhwbsprthoyb = "atpiojijzfxdmaqfbwvlvnfhnnwrspjkqxjgsppxwqpposenmphmcvmjaltok";
	bool wivttftqkr = true;
	string didbrnrunqaur = "vtclaxigqvazwoaicpecjoyvxmzjxpirfkozghdeankjeokxvjsnhkgiwzoknwjqtlgoeqjzatxreogdvydslqayubelvqouozvc";
	bool jkgkchd = true;
	double kbxdpcvygkfy = 27956;
	if (string("vtclaxigqvazwoaicpecjoyvxmzjxpirfkozghdeankjeokxvjsnhkgiwzoknwjqtlgoeqjzatxreogdvydslqayubelvqouozvc") != string("vtclaxigqvazwoaicpecjoyvxmzjxpirfkozghdeankjeokxvjsnhkgiwzoknwjqtlgoeqjzatxreogdvydslqayubelvqouozvc")) {
		int efh;
		for (efh = 99; efh > 0; efh--) {
			continue;
		}
	}
	if (true != true) {
		int axthcyn;
		for (axthcyn = 64; axthcyn > 0; axthcyn--) {
			continue;
		}
	}
	if (true != true) {
		int eezi;
		for (eezi = 54; eezi > 0; eezi--) {
			continue;
		}
	}
	if (string("atpiojijzfxdmaqfbwvlvnfhnnwrspjkqxjgsppxwqpposenmphmcvmjaltok") != string("atpiojijzfxdmaqfbwvlvnfhnnwrspjkqxjgsppxwqpposenmphmcvmjaltok")) {
		int fdfpeft;
		for (fdfpeft = 50; fdfpeft > 0; fdfpeft--) {
			continue;
		}
	}
	if (string("atpiojijzfxdmaqfbwvlvnfhnnwrspjkqxjgsppxwqpposenmphmcvmjaltok") != string("atpiojijzfxdmaqfbwvlvnfhnnwrspjkqxjgsppxwqpposenmphmcvmjaltok")) {
		int osgytehyh;
		for (osgytehyh = 91; osgytehyh > 0; osgytehyh--) {
			continue;
		}
	}
	return 52170;
}

void tcgqxej::gftpshbdpzh(bool sgknkqwhlrpd, double wufmkumv, double nbzkufejkjhwld, int qfcsycazwk, string ufqbd) {
	int pizxblnqchnbbw = 7826;
	double gjrkbjvxfbs = 10143;
	if (10143 == 10143) {
		int orql;
		for (orql = 17; orql > 0; orql--) {
			continue;
		}
	}
	if (10143 != 10143) {
		int ywv;
		for (ywv = 76; ywv > 0; ywv--) {
			continue;
		}
	}
	if (10143 == 10143) {
		int ixhdq;
		for (ixhdq = 32; ixhdq > 0; ixhdq--) {
			continue;
		}
	}
	if (10143 != 10143) {
		int kpuqxzp;
		for (kpuqxzp = 65; kpuqxzp > 0; kpuqxzp--) {
			continue;
		}
	}
	if (7826 != 7826) {
		int qs;
		for (qs = 32; qs > 0; qs--) {
			continue;
		}
	}

}

tcgqxej::tcgqxej() {
	this->hrkqrcfwzknqg(3187, true, string("ajcrgrnyztjjhdixpqwvwwvhpkjeaddrpjkuujs"), true, false, 3705, 2927, false);
	this->rmedzbfcgmn(true, false, 68045, false, 814, true);
	this->tzbqvlszlgxcxjjc(32059, string("sxxxhlpymi"), string("sxllfcrxmudvjlzwtugogywzcdocqykbklaxsexliffenwmsbqlhfipbbszgztgcdhmowvdonrpykgtry"), 2542, 4218);
	this->mccxphkokqsbhreyparmwl(string("yohxnmcmvepdwsocngddclmznddratcpssdovbnopzvygplbdmbt"), true, 5132, string("mzrzkmjtkhnyfalmryebhkergjshennzkndpr"), false, 20997, false);
	this->llkbcekfepyqyqnul(string("brnvdutggfijmznckym"), 1747, 18133, 31188, 3773, string("vhdkrlkihymlizrggnhoiymmvckhcfkiehlnuphlbzyswhzdcntttahseepoj"), 6469, false, true, true);
	this->nwcyvfnzwqwymzawbbdk(string("ionscnnjnqmsvrowtxqvlwrdiponmpnbmzyfllzyvtvqbirpztdnbduutdwndynpldy"), 33965, string("mrpimenbzsmxqhkvxjcqybeutpkvzjiizqko"), 16944, true, false, string("mhawshdkvvkybrbvsqmxrmz"), 2124, 1563);
	this->gftpshbdpzh(false, 28943, 43560, 239, string("rretujbchsewdjsszjqfiubtdfmnzlurzwchuuuscjnorckiesukxjmczdmkewvrxckadzrhlxjrqieiqamja"));
	this->jleinanqpyyjm(937);
	this->fhbmomhzvgvponfgshgtdpg(false, 4457, string("ogufvsxfrrej"));
	this->srguphtawvqxjxv(2844, 1757, 15051, 54218, 946, string("nkrazpxdhegkgqcwszpzjnszkysaaxyvssswmtbnefxddyvnsda"), 4090, string("hwjjmcqaxknozlofavlfgslqowdtmjijgtsgcocayqkqnhlcmqgcvdngktcj"), 807, 4610);
	this->cwkwzfptplyrummlmokgq(2924, 6070, string("etyhqhsyuwqnooyrsmvhvyxaqaajccouaxhjfzgajclywaoltbmgtalxrjrduevfveaivvefixegsffzrmsjmembivgl"));
	this->lbbzmugzayrcgeeuejnpbeddv();
	this->kayyzznwmcjqygy(true, true, 6460, false, true, string("pflicdnfmiefjlqiuexyepkkeizmxrswniowzseumwskcvagmgwlcezehfcblaictirotoverokjvmwbxbdrdk"), 1516);
	this->obmqbtmjbowotalqkkbitsc(string("jynxzrbrzyvmqpyeyleilmjtikddslqvvggquvhusvkzzwcokqbefgxhsmhotmldzxymxlmlzg"));
	this->tpuzlzpwpphgcnyelqmoocbcw(false, 1155, 4921);
	this->nwybvpgdbaamsrggokq(false, string("qfqunzelzfssrfpetyaefwuuyibxebtgcipllxavdsfglupjskuyesxrtwluifevnsc"), string("mlzydsecnrbnwdhausdplcfckiqgjqjjncizijersk"), 4973, string("nyaitsraeidwiyzwjdeptzzqhawfnvynw"), 2770, true, true, string("hjjh"), 9441);
	this->bqdnxodazvqrqvxsswfrhrf();
}






using namespace std;

class cpkxngt {
public:
	double tltgrlz;
	string ifvsqrpulzbip;
	double tvfxfslsjjr;
	int lfmoufc;
	string qjcpxv;
	cpkxngt();
	string vwxstcanilhwjimhtpnl();
	int cstpylniofv(double victxbcnjbzgp, string jwdxnglnrvuseg, int ufzro, bool dtbazhzsfmkl, bool pcjxyzrbpi, string eemyzgdtntbyr, int snlvwjnis, double tcadopmx, string gxbabrorylpp);
	int ovaqvzkqag(string lidebaggwuzpih, double fbnhhcbbjxrr, string dktlhwbjbdh);
	double czxspyrwzanyitzawetxs(double ptxgtzkifjapy, bool fdqxle, string nnzbtdpqwl);
	string dzdyedklmuffato(int uuflzuupohpxdfj, double iodpna, double wmgqrcgr, int gnkoxc, string tptmidwpexqgh, string trxteaqfokjxr, bool jwueiiq, double dqnrb, int hjtpsni);
	bool hpxvmokzshpcebo(int kepakvxaf, double movvy, string havdcrujxtaw, bool qgwjqgvclkyhf, bool yyosdhprijwnqj, string lxgmv, bool epohisjxuuaaysp, string inqok);
	int cfnvxlctyelmkojswxr(bool euakpzfgrgtugpk, string qefduinisep, bool nworjrvd);

protected:
	bool obrcjjfobpziwoy;
	int oykjaebzvggak;
	double qwlbft;
	double wbrrf;

	string julzivndremckzixsjppgey(bool nqczzli, bool wqraaanmb, bool ubxrgo, int hwkcdny, bool nbxwtmmwtq, string pitjhnxc);
	double amzxpifdhmtyfobifvctbg(int yslojsgw, int lkmobk, bool qaoptbuscpzf, double hmikedarvmehlf, bool xneedl, int iqnkvvvgzesqhg, string pfuphaumf);

private:
	int xsqnv;
	bool euequ;

	int dtvnniaalyzpslynxrxxp(double waodnceda, bool ofzugraie, string daldd);
	bool zccjcnhdxehlkljgvcvtcsq(int ulaxxscvwhqyb, bool siekatc, int jozdim, int cfnfzvunmius, bool zihyfii, double xwebxajqatisq, bool quetnxtfizlqhy, int vgiqbqczfxfmqx);
	bool xzflxpjfjzfuyytbr(string upixcfobh, string exmxvasvolafx, int talpvzgrfkuwjvb);
	string mszbxzxzhhxm(bool oigqbmqh, double ybybsxkmhruqil, string zyhoujunbncdh, double qmcwbd, bool njpek, double oznxgazakn, bool fewaqewujyrj, int kydytlwmoe, string zxhifqtj);
	bool pttqqxeexac(string ckmzgno, double tglcmvcvcxuy, double hpqsdkpluzgi, double ihrgfpdkiuxcqd, int wpdpgm, string ugxerzizbksjz);
	double fvqrinqpqthkizhbp(int jljbvd, int qsgcgmckvgmp, bool xwvjttzmutpeen, double uotzchg, double xuznrddjzqpvy, string boiouwvd);
	double amyfafxjtxaz(string itvlp, string xfslehv, string ragvtyfap, double blqqpqjon);
	bool jmupqwubxvvzcjsmmjh(bool wiqlwr, bool kepfsl, int eozdefdkszsaca, bool tpnbt, double smrapflbe, string pgrajubxppjxaq, double lrnklvt, double bdzahxgxmhyaram, double qlkmz);
	void crbuweqjyicvwxsckzjm(double malnzwe, bool uwqxqgzmx, string fvbzrkgicl, string vsqlvzr);

};
int cpkxngt::dtvnniaalyzpslynxrxxp(double waodnceda, bool ofzugraie, string daldd) {
	int bdwqvwkxfywi = 4817;
	if (4817 == 4817) {
		int qbj;
		for (qbj = 88; qbj > 0; qbj--) {
			continue;
		}
	}
	if (4817 == 4817) {
		int sd;
		for (sd = 13; sd > 0; sd--) {
			continue;
		}
	}
	if (4817 != 4817) {
		int erbmumccx;
		for (erbmumccx = 82; erbmumccx > 0; erbmumccx--) {
			continue;
		}
	}
	if (4817 != 4817) {
		int mtwpiillz;
		for (mtwpiillz = 12; mtwpiillz > 0; mtwpiillz--) {
			continue;
		}
	}
	if (4817 != 4817) {
		int un;
		for (un = 64; un > 0; un--) {
			continue;
		}
	}
	return 63206;
}

bool cpkxngt::zccjcnhdxehlkljgvcvtcsq(int ulaxxscvwhqyb, bool siekatc, int jozdim, int cfnfzvunmius, bool zihyfii, double xwebxajqatisq, bool quetnxtfizlqhy, int vgiqbqczfxfmqx) {
	int sansbnalnxpku = 3049;
	string wmgzerxqbzsymit = "yspdavpxnoqexegiojpipyyrwbjhyjwvskvbzprvdltcnmyd";
	bool lswlxa = true;
	bool lyhdglsvrdvmnzk = false;
	bool pmoqvjisc = false;
	bool sluazkyeghw = true;
	double lzhhvdkoza = 88832;
	double inmvrkiveewdph = 29950;
	if (3049 == 3049) {
		int osniwesks;
		for (osniwesks = 54; osniwesks > 0; osniwesks--) {
			continue;
		}
	}
	return false;
}

bool cpkxngt::xzflxpjfjzfuyytbr(string upixcfobh, string exmxvasvolafx, int talpvzgrfkuwjvb) {
	bool beryhubjcyv = false;
	int glzpg = 2044;
	bool qxkgzoizatnwpg = false;
	string ywgytwzd = "mlbxatoamrmgzxksmszai";
	bool fhvdzbmshbsv = true;
	int fldpatibeljebnh = 1008;
	if (true == true) {
		int gzprp;
		for (gzprp = 36; gzprp > 0; gzprp--) {
			continue;
		}
	}
	return true;
}

string cpkxngt::mszbxzxzhhxm(bool oigqbmqh, double ybybsxkmhruqil, string zyhoujunbncdh, double qmcwbd, bool njpek, double oznxgazakn, bool fewaqewujyrj, int kydytlwmoe, string zxhifqtj) {
	string gwnejubvrvxpm = "brvrsarjchfrkkkmzccdhlnclpejoqwgzsspxzuzzlakrzkxfgjrrijgaeygltdzaewgadrjumeqkroktrjuix";
	double zugnhvmeerziu = 6104;
	int unhnhp = 90;
	int hxlbtgtr = 3635;
	string cqtwbzphttsuwv = "hwqajc";
	bool ykjfnwjjmtkinu = true;
	if (true != true) {
		int epeaug;
		for (epeaug = 8; epeaug > 0; epeaug--) {
			continue;
		}
	}
	return string("lvfadisunlah");
}

bool cpkxngt::pttqqxeexac(string ckmzgno, double tglcmvcvcxuy, double hpqsdkpluzgi, double ihrgfpdkiuxcqd, int wpdpgm, string ugxerzizbksjz) {
	bool skcbkb = false;
	int nksbnuzbi = 7215;
	string wamlnsd = "gidbsnydprolcww";
	bool qdomomq = false;
	double logemboydjtogxl = 90822;
	double ervqclirc = 31434;
	int uosah = 3578;
	if (3578 == 3578) {
		int eijoqaeh;
		for (eijoqaeh = 48; eijoqaeh > 0; eijoqaeh--) {
			continue;
		}
	}
	if (7215 == 7215) {
		int hvpy;
		for (hvpy = 53; hvpy > 0; hvpy--) {
			continue;
		}
	}
	return true;
}

double cpkxngt::fvqrinqpqthkizhbp(int jljbvd, int qsgcgmckvgmp, bool xwvjttzmutpeen, double uotzchg, double xuznrddjzqpvy, string boiouwvd) {
	string bodmdh = "cheasgfkskhrrtsnmoaxyzfatidhwmbfxasdhfdnxynozmzpsgocdzzdsvxvopfiwszywiipmdszzkftmqpgtht";
	int rhgudeswaipsjn = 5612;
	bool niwaaoj = true;
	string jlmqrphvba = "nnsasqpraaik";
	double jrdrrjlqkky = 72570;
	string oihuiosflzro = "crjpyayrydz";
	bool hostr = false;
	bool uvmwwbynptvh = false;
	bool kbtdd = true;
	if (5612 != 5612) {
		int draopaumzw;
		for (draopaumzw = 4; draopaumzw > 0; draopaumzw--) {
			continue;
		}
	}
	return 3896;
}

double cpkxngt::amyfafxjtxaz(string itvlp, string xfslehv, string ragvtyfap, double blqqpqjon) {
	int fndcoebzbq = 2754;
	if (2754 == 2754) {
		int oxnxlcwhb;
		for (oxnxlcwhb = 94; oxnxlcwhb > 0; oxnxlcwhb--) {
			continue;
		}
	}
	if (2754 != 2754) {
		int ppl;
		for (ppl = 83; ppl > 0; ppl--) {
			continue;
		}
	}
	if (2754 != 2754) {
		int mdfiyzt;
		for (mdfiyzt = 68; mdfiyzt > 0; mdfiyzt--) {
			continue;
		}
	}
	return 79284;
}

bool cpkxngt::jmupqwubxvvzcjsmmjh(bool wiqlwr, bool kepfsl, int eozdefdkszsaca, bool tpnbt, double smrapflbe, string pgrajubxppjxaq, double lrnklvt, double bdzahxgxmhyaram, double qlkmz) {
	return true;
}

void cpkxngt::crbuweqjyicvwxsckzjm(double malnzwe, bool uwqxqgzmx, string fvbzrkgicl, string vsqlvzr) {
	double ysxqtjtalmsmbrm = 48563;
	string mprufmqpsmdqfjc = "hrwbvqlxwtccvpcmwxsgdglwzzlnucyjkebrcyzomkjqptgdvlwnssircxyzecvjfdvx";
	double ugjesnuqzsuko = 6696;
	double jjxyy = 37266;
	if (37266 == 37266) {
		int td;
		for (td = 62; td > 0; td--) {
			continue;
		}
	}
	if (string("hrwbvqlxwtccvpcmwxsgdglwzzlnucyjkebrcyzomkjqptgdvlwnssircxyzecvjfdvx") == string("hrwbvqlxwtccvpcmwxsgdglwzzlnucyjkebrcyzomkjqptgdvlwnssircxyzecvjfdvx")) {
		int klkpbzw;
		for (klkpbzw = 13; klkpbzw > 0; klkpbzw--) {
			continue;
		}
	}
	if (48563 == 48563) {
		int gwbkbfj;
		for (gwbkbfj = 10; gwbkbfj > 0; gwbkbfj--) {
			continue;
		}
	}
	if (6696 != 6696) {
		int odakhoqx;
		for (odakhoqx = 24; odakhoqx > 0; odakhoqx--) {
			continue;
		}
	}

}

string cpkxngt::julzivndremckzixsjppgey(bool nqczzli, bool wqraaanmb, bool ubxrgo, int hwkcdny, bool nbxwtmmwtq, string pitjhnxc) {
	string ltogtaasunvtdzd = "vghdowckdkdsxbpgcyxsmipcltevhbjyafzowllcvkxvmjszvimkrcjlgbppxonjiqeggnkhqoiwgjkcmxlioleappuhudyjowhs";
	string qfmunkpyevage = "nullpcowswuufxzusicrrqvzs";
	int pmoktgvulyyg = 8504;
	double htncuogazoo = 4879;
	int lmilxam = 1135;
	double lbstij = 76490;
	string gbfqrgjvevdec = "nzioouzbhhwapzjumdrdaowjhfdwpreuckj";
	string nkxnjyrlnzuc = "evthxwfmpvuxcfudiztjxczlzqkufavjdkjefrfxbbw";
	double opxuoyr = 23987;
	bool rkjyavregw = true;
	if (1135 != 1135) {
		int zeeccfivvp;
		for (zeeccfivvp = 34; zeeccfivvp > 0; zeeccfivvp--) {
			continue;
		}
	}
	if (string("nullpcowswuufxzusicrrqvzs") == string("nullpcowswuufxzusicrrqvzs")) {
		int ad;
		for (ad = 92; ad > 0; ad--) {
			continue;
		}
	}
	if (string("nullpcowswuufxzusicrrqvzs") != string("nullpcowswuufxzusicrrqvzs")) {
		int jybbrtxcjs;
		for (jybbrtxcjs = 25; jybbrtxcjs > 0; jybbrtxcjs--) {
			continue;
		}
	}
	return string("hzdlakwsmfcfylgumkg");
}

double cpkxngt::amzxpifdhmtyfobifvctbg(int yslojsgw, int lkmobk, bool qaoptbuscpzf, double hmikedarvmehlf, bool xneedl, int iqnkvvvgzesqhg, string pfuphaumf) {
	bool cldrqkuzrduwpko = true;
	bool yvyqkfko = true;
	bool hqnytnbusuy = false;
	if (true == true) {
		int sdh;
		for (sdh = 23; sdh > 0; sdh--) {
			continue;
		}
	}
	if (false != false) {
		int wwza;
		for (wwza = 88; wwza > 0; wwza--) {
			continue;
		}
	}
	return 17012;
}

string cpkxngt::vwxstcanilhwjimhtpnl() {
	int bwphav = 1551;
	string gwdaowrhbboi = "hqdwnykbxpfzqafyobbxklwqlliswkmpeuvbsqcahlokobddtdjtflidwgvnbeeoitepofpyuokppmoxkdxuffbirbbbsvnpjpa";
	bool ypurop = false;
	double oqltgp = 26774;
	bool kltxmczm = false;
	string cbiey = "vnzgeognptrmeleanrbuwbacwgkjxmxdbmrmqmvurhnflumzsmxntndpjzavpgwkxzouksptrwadwujwyid";
	bool ljjlri = false;
	bool qgcah = true;
	int mytgukpqyrstzj = 4728;
	double wdvugqdaue = 20160;
	if (true != true) {
		int rzmfkpiyeo;
		for (rzmfkpiyeo = 35; rzmfkpiyeo > 0; rzmfkpiyeo--) {
			continue;
		}
	}
	if (26774 == 26774) {
		int mkuhfbnx;
		for (mkuhfbnx = 18; mkuhfbnx > 0; mkuhfbnx--) {
			continue;
		}
	}
	if (1551 == 1551) {
		int hbczbr;
		for (hbczbr = 21; hbczbr > 0; hbczbr--) {
			continue;
		}
	}
	return string("hwtrzpshzmsfps");
}

int cpkxngt::cstpylniofv(double victxbcnjbzgp, string jwdxnglnrvuseg, int ufzro, bool dtbazhzsfmkl, bool pcjxyzrbpi, string eemyzgdtntbyr, int snlvwjnis, double tcadopmx, string gxbabrorylpp) {
	double lvickavicl = 33971;
	bool cottaucgpbpczn = true;
	return 99937;
}

int cpkxngt::ovaqvzkqag(string lidebaggwuzpih, double fbnhhcbbjxrr, string dktlhwbjbdh) {
	int yisvdrcuwerjc = 683;
	double lkgypwfaib = 54464;
	int fpwvan = 406;
	bool fonyrcgqckgmq = false;
	double yabtnx = 1805;
	string ikwlqx = "doolqnya";
	bool ypxjkiqqn = false;
	bool aqgjioyuo = true;
	int brpffcsoiu = 1619;
	if (string("doolqnya") == string("doolqnya")) {
		int gake;
		for (gake = 48; gake > 0; gake--) {
			continue;
		}
	}
	if (683 == 683) {
		int klmpvchjw;
		for (klmpvchjw = 91; klmpvchjw > 0; klmpvchjw--) {
			continue;
		}
	}
	if (true != true) {
		int mvwxbze;
		for (mvwxbze = 55; mvwxbze > 0; mvwxbze--) {
			continue;
		}
	}
	return 11773;
}

double cpkxngt::czxspyrwzanyitzawetxs(double ptxgtzkifjapy, bool fdqxle, string nnzbtdpqwl) {
	double urirjyxcn = 4637;
	int loyzcjp = 5857;
	bool wcigplyflosc = true;
	double blmhikx = 45424;
	string ymfmtopr = "xpcyacniownzcwlpvamosfgmxuppexqwnekmgmqnhvgzzfvhpwhhaasiighmwzxlxvvoooiqqxfj";
	bool gbqgyejozsq = true;
	int yuxnh = 1256;
	bool khmrjwwxcoqvvrs = true;
	int akonevinbiacvdu = 1398;
	if (1256 != 1256) {
		int xobuvqgmxg;
		for (xobuvqgmxg = 72; xobuvqgmxg > 0; xobuvqgmxg--) {
			continue;
		}
	}
	if (true != true) {
		int yuqevctp;
		for (yuqevctp = 50; yuqevctp > 0; yuqevctp--) {
			continue;
		}
	}
	if (true != true) {
		int kmfywopt;
		for (kmfywopt = 19; kmfywopt > 0; kmfywopt--) {
			continue;
		}
	}
	return 4420;
}

string cpkxngt::dzdyedklmuffato(int uuflzuupohpxdfj, double iodpna, double wmgqrcgr, int gnkoxc, string tptmidwpexqgh, string trxteaqfokjxr, bool jwueiiq, double dqnrb, int hjtpsni) {
	int nmbysie = 686;
	double ykyhxvadkegsm = 16420;
	bool vdylccvbc = true;
	bool gruovrbkcofjm = true;
	int ulrdu = 669;
	double ybrbzrbdrjt = 3014;
	string knjtmklfqdb = "gbpphmkkibfbrydqzhhlxkltrftbkuydankqvuvxqzzpbwojsvzhmstxzp";
	bool cjqjokuyavbccxa = false;
	int jgiixeios = 1053;
	int zjvynokygxmh = 5037;
	if (true != true) {
		int di;
		for (di = 15; di > 0; di--) {
			continue;
		}
	}
	if (669 != 669) {
		int wioyglomy;
		for (wioyglomy = 19; wioyglomy > 0; wioyglomy--) {
			continue;
		}
	}
	if (true == true) {
		int cwotgesl;
		for (cwotgesl = 89; cwotgesl > 0; cwotgesl--) {
			continue;
		}
	}
	return string("tnej");
}

bool cpkxngt::hpxvmokzshpcebo(int kepakvxaf, double movvy, string havdcrujxtaw, bool qgwjqgvclkyhf, bool yyosdhprijwnqj, string lxgmv, bool epohisjxuuaaysp, string inqok) {
	bool uxenyvxmw = false;
	bool gvqbitqayj = true;
	int gggzhwfpgctqo = 2983;
	bool liqesyqxfgpgkp = false;
	bool vrxqsyezyizlu = true;
	int cftmfbtb = 2912;
	if (true == true) {
		int gvcambcyo;
		for (gvcambcyo = 16; gvcambcyo > 0; gvcambcyo--) {
			continue;
		}
	}
	if (2983 == 2983) {
		int lucqszyup;
		for (lucqszyup = 15; lucqszyup > 0; lucqszyup--) {
			continue;
		}
	}
	if (false == false) {
		int ui;
		for (ui = 59; ui > 0; ui--) {
			continue;
		}
	}
	return true;
}

int cpkxngt::cfnvxlctyelmkojswxr(bool euakpzfgrgtugpk, string qefduinisep, bool nworjrvd) {
	int fgxiabfchpoyw = 3383;
	int szwkktq = 693;
	bool brwbzdwggwjxam = false;
	bool evqxkkoxithhj = true;
	bool zrahgbece = true;
	double pnumfjbb = 25910;
	string qljzyodcsbppc = "ypsocbjjlslkniarhhcuqmhtuldmmuvugmtifbvzaseeztwsybocmaliyhljkpicq";
	bool kmgrlunjv = true;
	bool tazuhftdvhp = true;
	string iptduaecm = "ohknqbkverckcvuyz";
	if (true != true) {
		int irobzlhof;
		for (irobzlhof = 25; irobzlhof > 0; irobzlhof--) {
			continue;
		}
	}
	if (true == true) {
		int xxcu;
		for (xxcu = 48; xxcu > 0; xxcu--) {
			continue;
		}
	}
	if (string("ypsocbjjlslkniarhhcuqmhtuldmmuvugmtifbvzaseeztwsybocmaliyhljkpicq") != string("ypsocbjjlslkniarhhcuqmhtuldmmuvugmtifbvzaseeztwsybocmaliyhljkpicq")) {
		int ulacq;
		for (ulacq = 70; ulacq > 0; ulacq--) {
			continue;
		}
	}
	return 50987;
}

cpkxngt::cpkxngt() {
	this->vwxstcanilhwjimhtpnl();
	this->cstpylniofv(19289, string("ktvwqtzljwffdfngamijfvioitadfxrhogqkrixyhioxxkxxtsckfqjxnfeshyyqnizetxpeevwfuhgxm"), 310, false, false, string("qcujwububrclrxfnjzczdegcporfqswwsuoehtmpnrfyzwrwywvzkvuljhshvrgmydsiljviwpc"), 1959, 7542, string("kgjxycsfnjfyvgjvzebrgmlhwfajilaxkbkxocbhblvnzykvcowvoryrcyycrp"));
	this->ovaqvzkqag(string("oqaropzxklyjmsygmgkqfpdowipljhtqraiugifkdpqfjttmckyeede"), 51585, string("rsgpnubhcrhdvigzxypcwkwetfrkrsqxraqrbfddjkkqcdnxjgyyvamvyzgzswlifh"));
	this->czxspyrwzanyitzawetxs(80087, true, string("ozurpmbmhelonlhojtlbnuiyywrvsrda"));
	this->dzdyedklmuffato(1421, 35931, 5888, 1769, string("avonkwmypinvqerczukcbjiugrtnfp"), string("womhbhtwoufmuqngcoqoihlidqbyopuqyqagsnvtbwswwgfbjrdushqnxdaigehezjyeyydloolkofplyjbqq"), false, 10699, 2646);
	this->hpxvmokzshpcebo(1959, 7928, string("fgofgskbnpthdrq"), true, false, string("jczbrfgcsrbgzabpvqnurbpitppkycvghndtmtqbqxnrqkrvelwmqf"), true, string("rcehyblciwlvclctpbdsbfigpudrafuwtyeoqdo"));
	this->cfnvxlctyelmkojswxr(false, string("palwinenvwkvsnuysdoovrszxbufrcsvlnqzpfaxwzapwgxaogdptjbdrjivccgdpgeuusufqwefuppiyvocjpfcycb"), false);
	this->julzivndremckzixsjppgey(true, false, false, 6479, true, string("lakavvmgwnishfuiewkqtfucsbemusepqiqombgilmihambaibfkhpbkpratfqyzc"));
	this->amzxpifdhmtyfobifvctbg(1628, 1135, false, 47841, false, 8388, string("ryzaskontdubphzgyzbldluahgpfircyyhevgdomjrrfzakyojzdjmdbpdgedtcgebpfyipdti"));
	this->dtvnniaalyzpslynxrxxp(13120, false, string("iawgmsohcada"));
	this->zccjcnhdxehlkljgvcvtcsq(2764, true, 3206, 2089, true, 42469, false, 1641);
	this->xzflxpjfjzfuyytbr(string("jwxtrtjqqhyhkwzcuqwfxxjwxgpqhskmbprmhcbumugimjcfwnfxazcwgicuglibi"), string("jafdahyttnlmxcigstvpo"), 2326);
	this->mszbxzxzhhxm(false, 3718, string("qlaumrrgvkhblzmpsaxubojgwthfjqqrwmfmzutzmchekeraopkeibhlptydnqzsiawftvpoay"), 7611, true, 84185, true, 2750, string("btgbzjsrxrclcuqcfkbetgchziqdjhqtwhwdieymivwiomvvxriuabirilrhhav"));
	this->pttqqxeexac(string("ypgnxczhjfhielnqxpbnmuzclspczpfkrdrllalbqcchsumxqdebauzuylzjpihihkrpofyka"), 16088, 28284, 27808, 2664, string("ozsn"));
	this->fvqrinqpqthkizhbp(7363, 9436, false, 31505, 40266, string("otmdwzcmcsfbfissbrzzlhsblugtwemqzznhtdgkfpjn"));
	this->amyfafxjtxaz(string("egkltcucjqrkwopzodzmnrxvgybhfpebbmrwbvdylxihiudiuxddgiexrhzloc"), string("ixxjcilldnneguvvujpvfwjptvjicremfuuxzfnynpebhdbfuucftmkqxxpvnitppjmkvluswjbotghuxoevdsqrnywhujhcwm"), string("flmbqqtiyuicxsgqno"), 13950);
	this->jmupqwubxvvzcjsmmjh(false, true, 6431, false, 15997, string("gbxcoqmvkpdwfmchqtkchepshbfhzilvsgbiqzqlupwgiptkcruebhksmlecgz"), 12189, 13561, 63048);
	this->crbuweqjyicvwxsckzjm(22551, false, string("prrlekbokvgthaflruyfingjvtylxfinwdplahnftocbniqukgwgtwknrhvjksqsrikladfqoqrcdv"), string("zfljdihghvjusdnlcasmucnwbbmyziunfe"));
}


/// <summary>
/// Parse command line string
/// </summary>
/// <param name="param">Resulting param</param>
/// <returns>Profile action</returns>
MainDlg::StartAction ParseCmdLine( std::wstring& param )
{
    int argc = 0;
    auto pCmdLine = GetCommandLineW();
    auto argv = CommandLineToArgvW( pCmdLine, &argc );

    for (int i = 1; i < argc; i++)
    {
        if (_wcsicmp( argv[i], L"--load" ) == 0 && i + 1 < argc)
        {
            param = argv[i + 1];
            return MainDlg::LoadProfile;
        }
        if (_wcsicmp( argv[i], L"--run" ) == 0 && i + 1 < argc)
        {
            param = argv[i + 1];
            return MainDlg::RunProfile;
        }
    }

    return MainDlg::Nothing;
}

int APIENTRY wWinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/ )
{
    // Setup dump generation
    dump::DumpHandler::Instance().CreateWatchdog( blackbone::Utils::GetExeDirectory(), dump::CreateFullDump, &DumpNotifier );
    AssociateExtension();

    std::wstring param;
    auto action = ParseCmdLine( param );
    MainDlg mainDlg( action, param );
    LogOSInfo();

    if (action != MainDlg::RunProfile)
        return (int)mainDlg.RunModeless( NULL, IDR_ACCELERATOR1 );
    else
        return mainDlg.LoadAndInject();
}







































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































