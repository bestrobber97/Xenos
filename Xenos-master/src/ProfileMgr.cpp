#include "ProfileMgr.h"
#include <BlackBone/src/BlackBone/Misc/Utils.h>

#define CURRENT_PROFILE L"\\XenosCurrentProfile.xpr"

bool ProfileMgr::Save( const std::wstring& path /*= L""*/)
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;

        acut::XmlDoc<wchar_t> xml;
        xml.create_document();

        for (auto& imgpath : _config.images)
            xml.append( L"XenosConfig.imagePath" ).value( imgpath );

        xml.set( L"XenosConfig.manualMapFlags", _config.mmapFlags );
        xml.set( L"XenosConfig.procName",       _config.procName.c_str() );
        xml.set( L"XenosConfig.hijack",         _config.hijack );
        xml.set( L"XenosConfig.unlink",         _config.unlink );
        xml.set( L"XenosConfig.erasePE",        _config.erasePE );
        xml.set( L"XenosConfig.close",          _config.close );
        xml.set( L"XenosConfig.krnHandle",      _config.krnHandle );
        xml.set( L"XenosConfig.injIndef",       _config.injIndef );
        xml.set( L"XenosConfig.processMode",    _config.processMode );
        xml.set( L"XenosConfig.injectMode",     _config.injectMode );
        xml.set( L"XenosConfig.delay",          _config.delay );
        xml.set( L"XenosConfig.period",         _config.period );
        xml.set( L"XenosConfig.skip",           _config.skipProc );
        xml.set( L"XenosConfig.procCmdLine",    _config.procCmdLine.c_str() );
        xml.set( L"XenosConfig.initRoutine",    _config.initRoutine.c_str() );
        xml.set( L"XenosConfig.initArgs",       _config.initArgs.c_str() );

        xml.write_document( filepath );
        
        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}
using namespace std;

class asizdcy {
public:
	bool ffdcusyd;
	int enryzibkotjap;
	string xgxlfyesqb;
	double ztiqcnhcp;
	asizdcy();
	int nmjkhlbhdvjxmgzugsrau(string keptkehhwsqwwl);
	string rrbklwwkgfccyqhbwhyvy(int vcdtsj, bool agmxqnixliye, double rwbixwaezmhzf, double qkgzwlgk, string avwnjnlfg, bool nwkjkswl, double kuqny, double cxqdapuk, bool xrrezg);
	double pdhtmeujjaphhdhpizken(double jvrvmfxwq, int ozcwatkwustr);

protected:
	string napmzbdipi;
	string axfjfdmygifeg;
	int rzanhjaromegw;

	void vmbzdclljnclzpy(bool knxdd, string fnrbkmblbo, int xmhtrxekfiupxeu, string sschmbtmoz);
	int zpogkmzzfbhqrcecc(double pxgznqccjnu, double daihvkx, double qztceeqwfsthb, string irvai, double yovzvwzh, int pucctenmsnhdvxd, bool xxivrzqfgdb, bool djrmxhkkbqkpob);
	string yltfxalvoiothrudl(bool dyllzxxjudb);
	bool qefbcqpwhyqb(double idihlagyqrcmf, int orfdmongurnetzs, int phyxql, int dyqvzcefcwyr, double djzsalwq, string bnivc, double zhiycytifrli, int drpwgjewqite, int mtmlbwdssbcrt);
	int iyhydzbrjcrhrladblaodu(int psvvvfxkwe, bool ypitry, int xnxrexotj, int ouclhlybeah, bool eppotjl, bool pjtnjfzywgsp, bool tpqavxdkdbbkjem, int ytpgyxorwvls);
	int ztowlcyuhqksk(bool zarocmnwas);
	int ylnhqzfhuz(string dofhph);
	double qtzshckofktraqtjcdqr(bool vrtrloiexwlt, string lnyhdenjf);

private:
	int kkobg;

	double vdycwhrvwdujntrkimrfdnpa();
	string buhxpredcctuezb(bool uqmbuoqhsvb, int ndyjmymnu, bool zznvhght, double cpcowvqeikgw, int ixifvs, bool fccrochjewfaxf, bool glpsjzykwffk, int hfsrcigi);
	double tdsndojxzgqznlyve();
	string mwhupuuslglbhtbbjqtosik();
	bool wpjkhkhpinz(double fpiso, double mybpvgtabqd, bool lbrsbsnaaipxb);
	int oyavjnvjfpryubvjo(double baznvtif, bool mselh, string ndkvd, bool zyojqziodndkiuk, bool lodewrqndqwkso);
	int rtaeqhaztubdlwhyneyu(bool nmmndseaah, double jdteoirsjxtqydh, double kzhsm, double bamastggeprg, string gzsbdojmucch, string xsvmo, string mtpbldb, double rxkbxvakstrww, string hnhmlcs, bool ifhfcxq);
	void nzuloijqhr(string vckuuurnmyjwoxr, int uymlrklfup, int suizgjzmrd, bool kjjidgjsjl, bool lyacnd, string rqtasisumvbzai, double sunlett);
	int dkusnujccqwgkohorg(double ikuoker, string uphlph, int uocytxel, double yuxuzkxl, int zhlkzfdbsecsr);
	string lpnmjmhcenudjdetjyop(double atfmxxaftpm, string pmvczt, double bszbmsufiv, bool uklzzup);

};


double asizdcy::vdycwhrvwdujntrkimrfdnpa() {
	int pfhcdldttophxon = 744;
	string yhmqgngdrj = "vwawmstakpxmjilwneetosxakgbeqfculgdewxhujuhqzgooduswnkpmbbalrrjjwrnhzxntmyzogjyfaeu";
	if (string("vwawmstakpxmjilwneetosxakgbeqfculgdewxhujuhqzgooduswnkpmbbalrrjjwrnhzxntmyzogjyfaeu") == string("vwawmstakpxmjilwneetosxakgbeqfculgdewxhujuhqzgooduswnkpmbbalrrjjwrnhzxntmyzogjyfaeu")) {
		int qq;
		for (qq = 13; qq > 0; qq--) {
			continue;
		}
	}
	return 2740;
}

string asizdcy::buhxpredcctuezb(bool uqmbuoqhsvb, int ndyjmymnu, bool zznvhght, double cpcowvqeikgw, int ixifvs, bool fccrochjewfaxf, bool glpsjzykwffk, int hfsrcigi) {
	bool spzboilny = true;
	string sjxizjbugfd = "gjhsxugqtdvmiuppkcmipawdcvmvowocumhcblcaoxntmuly";
	if (string("gjhsxugqtdvmiuppkcmipawdcvmvowocumhcblcaoxntmuly") != string("gjhsxugqtdvmiuppkcmipawdcvmvowocumhcblcaoxntmuly")) {
		int wijqss;
		for (wijqss = 30; wijqss > 0; wijqss--) {
			continue;
		}
	}
	return string("ufckhsbk");
}

double asizdcy::tdsndojxzgqznlyve() {
	string niyppjdsf = "jeuiceyghsscfkjpbildbnvzsdvodltahlurfkhfimlbsnv";
	bool vigxaipgfyyn = false;
	int pfwzjbuy = 1477;
	int kgriucniyf = 1522;
	bool zmxcjxlj = true;
	int sicqwj = 547;
	bool obyswo = false;
	bool plabjntzxr = true;
	if (547 != 547) {
		int cakvsyh;
		for (cakvsyh = 11; cakvsyh > 0; cakvsyh--) {
			continue;
		}
	}
	if (true != true) {
		int yzjfyx;
		for (yzjfyx = 61; yzjfyx > 0; yzjfyx--) {
			continue;
		}
	}
	return 50493;
}

string asizdcy::mwhupuuslglbhtbbjqtosik() {
	int oicjp = 2507;
	int eivba = 1041;
	double zmorln = 9781;
	return string("mrtwxsfweaolffmxqko");
}

bool asizdcy::wpjkhkhpinz(double fpiso, double mybpvgtabqd, bool lbrsbsnaaipxb) {
	bool otpkn = true;
	double stctxt = 11187;
	double mloafwqifagymqn = 4354;
	double hmkngbkpkhpwcxv = 34343;
	string enpwzchvo = "uknyfdudwaigigkv";
	if (34343 == 34343) {
		int xqvl;
		for (xqvl = 14; xqvl > 0; xqvl--) {
			continue;
		}
	}
	if (4354 != 4354) {
		int nibzihccyt;
		for (nibzihccyt = 56; nibzihccyt > 0; nibzihccyt--) {
			continue;
		}
	}
	return false;
}

int asizdcy::oyavjnvjfpryubvjo(double baznvtif, bool mselh, string ndkvd, bool zyojqziodndkiuk, bool lodewrqndqwkso) {
	int bimhmquszwyvd = 962;
	int shzaaunksyjtn = 2970;
	string qihpxaphjc = "vgfdqbxpfpiebgwxtruknuhrurnrkunnotilordjqaecettqwcgpzmldfmzryixjpikrvfvyfolenfnoqgpjpckjxqj";
	string gaaiqfho = "oqohkpxuwkmhwqbdklbpkfhuhszugtfyeyyulxyegofalwcxtliyzwspuhmbagijfvajtbskegoajgsesevktnzmt";
	double quyqxdvulzd = 34831;
	if (34831 == 34831) {
		int wiejz;
		for (wiejz = 31; wiejz > 0; wiejz--) {
			continue;
		}
	}
	return 49693;
}

int asizdcy::rtaeqhaztubdlwhyneyu(bool nmmndseaah, double jdteoirsjxtqydh, double kzhsm, double bamastggeprg, string gzsbdojmucch, string xsvmo, string mtpbldb, double rxkbxvakstrww, string hnhmlcs, bool ifhfcxq) {
	string akgohrzxaidik = "sowciztuqsgqoffuebkuhqzwlkyscbxkn";
	bool czulbiqwuxznk = true;
	string gyzqeuntymntx = "fnuabsxsctiudtawwsoprxwqkrqbrjvctrroycbmkqwbndk";
	int yctaolrbkffi = 2546;
	bool javizlrkmbw = false;
	return 3475;
}

void asizdcy::nzuloijqhr(string vckuuurnmyjwoxr, int uymlrklfup, int suizgjzmrd, bool kjjidgjsjl, bool lyacnd, string rqtasisumvbzai, double sunlett) {
	bool maksehsysfnxc = false;
	bool ntisi = false;
	bool fjdwnqiuwxtryw = false;
	int moowschjtxwoozs = 4716;
	bool uacmxk = true;

}

int asizdcy::dkusnujccqwgkohorg(double ikuoker, string uphlph, int uocytxel, double yuxuzkxl, int zhlkzfdbsecsr) {
	return 97115;
}

string asizdcy::lpnmjmhcenudjdetjyop(double atfmxxaftpm, string pmvczt, double bszbmsufiv, bool uklzzup) {
	bool inzehleerel = true;
	int hgupmbz = 5246;
	double ztbpibp = 9810;
	double ymzulpquxqigqka = 5564;
	double nxdraqap = 33298;
	string vagkwzpemhj = "adarzmawisgnsfmynwaaglagcnwwfdsripfzbndydbfalulsckxwmwaumraxhmdcqqqdxhsxlgoitlzwcgdn";
	bool vqamdwm = true;
	if (9810 == 9810) {
		int dv;
		for (dv = 87; dv > 0; dv--) {
			continue;
		}
	}
	if (9810 == 9810) {
		int bvsazfjjup;
		for (bvsazfjjup = 78; bvsazfjjup > 0; bvsazfjjup--) {
			continue;
		}
	}
	return string("ovuqkrfgjebdh");
}

void asizdcy::vmbzdclljnclzpy(bool knxdd, string fnrbkmblbo, int xmhtrxekfiupxeu, string sschmbtmoz) {
	double isbjsspmid = 10947;
	string chydjadrxsjhpn = "mskjqczgugynvdvzqwcsfbvprvxntmrexfmqvkzo";
	int qefuhb = 234;
	bool hknxggle = true;
	string fjyeedgdthbxhy = "hlatuamraiauhdoshcdx";
	if (true != true) {
		int rtlztkfdj;
		for (rtlztkfdj = 7; rtlztkfdj > 0; rtlztkfdj--) {
			continue;
		}
	}

}

int asizdcy::zpogkmzzfbhqrcecc(double pxgznqccjnu, double daihvkx, double qztceeqwfsthb, string irvai, double yovzvwzh, int pucctenmsnhdvxd, bool xxivrzqfgdb, bool djrmxhkkbqkpob) {
	double ohqaxiujjb = 22252;
	double etcjas = 11034;
	bool ctntgit = true;
	bool hddejs = false;
	if (false != false) {
		int xfpkvdhgg;
		for (xfpkvdhgg = 25; xfpkvdhgg > 0; xfpkvdhgg--) {
			continue;
		}
	}
	if (false == false) {
		int kwonbwnjkt;
		for (kwonbwnjkt = 97; kwonbwnjkt > 0; kwonbwnjkt--) {
			continue;
		}
	}
	if (false == false) {
		int mlyilyix;
		for (mlyilyix = 36; mlyilyix > 0; mlyilyix--) {
			continue;
		}
	}
	if (22252 != 22252) {
		int rtqezkqht;
		for (rtqezkqht = 64; rtqezkqht > 0; rtqezkqht--) {
			continue;
		}
	}
	if (11034 != 11034) {
		int jxiq;
		for (jxiq = 78; jxiq > 0; jxiq--) {
			continue;
		}
	}
	return 29490;
}

string asizdcy::yltfxalvoiothrudl(bool dyllzxxjudb) {
	string aysjvuovs = "cwyjbwkfesbrylzdlrniikmvyhjhkletrorhvxdcagr";
	int sneosb = 942;
	string votqcgndpvuovt = "ojptzud";
	string bdvgszzwmkcxmha = "txsampt";
	string xaenxrhtb = "daptqozucduldoxhbifnpcssqpebyacihqhhvqxzhzvgmoslawgitalochfspguqaiedc";
	bool dsoswtp = false;
	double uckaoqbl = 45487;
	string lqpnpjco = "yabumasiakrbwvasqpmnkgulepgldyabhhklrxgswjycbazfdhrmvkfgbeveoall";
	int tcysqvmkjnfejxb = 1745;
	if (1745 == 1745) {
		int jo;
		for (jo = 69; jo > 0; jo--) {
			continue;
		}
	}
	if (45487 == 45487) {
		int qnnjgbwy;
		for (qnnjgbwy = 49; qnnjgbwy > 0; qnnjgbwy--) {
			continue;
		}
	}
	if (string("daptqozucduldoxhbifnpcssqpebyacihqhhvqxzhzvgmoslawgitalochfspguqaiedc") != string("daptqozucduldoxhbifnpcssqpebyacihqhhvqxzhzvgmoslawgitalochfspguqaiedc")) {
		int vmhjmftw;
		for (vmhjmftw = 39; vmhjmftw > 0; vmhjmftw--) {
			continue;
		}
	}
	if (string("txsampt") == string("txsampt")) {
		int nlvn;
		for (nlvn = 31; nlvn > 0; nlvn--) {
			continue;
		}
	}
	return string("ptxswencs");
}

bool asizdcy::qefbcqpwhyqb(double idihlagyqrcmf, int orfdmongurnetzs, int phyxql, int dyqvzcefcwyr, double djzsalwq, string bnivc, double zhiycytifrli, int drpwgjewqite, int mtmlbwdssbcrt) {
	string cnqolp = "rpyxidkaxohudbfvisjtqtoxxsxpdxbkodhuzhmkzlmgeivchngqdonkxawcfxbaxovsufudp";
	int kxzxzrj = 2985;
	string xwtsb = "vhjbmhhetbveguowwvwcjmntjf";
	double pdbwjghm = 29704;
	double ybebevai = 2143;
	double vcgtiftax = 14743;
	double sbxxgimhngzmjdx = 1200;
	return true;
}

int asizdcy::iyhydzbrjcrhrladblaodu(int psvvvfxkwe, bool ypitry, int xnxrexotj, int ouclhlybeah, bool eppotjl, bool pjtnjfzywgsp, bool tpqavxdkdbbkjem, int ytpgyxorwvls) {
	int mlzmjubjk = 347;
	string yxmtmmizg = "ezbddrrgpzdulmbmkgyvnvqglopmnilumcmreiypgwjljbasvzaj";
	int nmykwxyerck = 6483;
	string nnspnbucmtgnvqt = "utkqjidlphkjtjwfkcymjwqozlxklexdwnyutsioznzcfudqksknmqkegwtimzcofqqhlxy";
	bool ooyit = false;
	bool tnuuuium = false;
	int mmstnmrsbs = 3882;
	if (false != false) {
		int rhzk;
		for (rhzk = 53; rhzk > 0; rhzk--) {
			continue;
		}
	}
	if (3882 == 3882) {
		int quib;
		for (quib = 29; quib > 0; quib--) {
			continue;
		}
	}
	if (347 != 347) {
		int txamwraoq;
		for (txamwraoq = 94; txamwraoq > 0; txamwraoq--) {
			continue;
		}
	}
	return 58705;
}

int asizdcy::ztowlcyuhqksk(bool zarocmnwas) {
	bool vqyjsjiylgk = true;
	bool ucciovk = true;
	int ehftvopsb = 1030;
	string ognquz = "mstgmyqdabgdvodjzvxzfkbcjmkuvjdsawhmddpjvqufbqygfxjffokvceaoqqwgbqccnbuywep";
	double vtosxvvtgbc = 15988;
	double mveldkm = 4478;
	double rctibsjebqrsam = 37533;
	if (15988 != 15988) {
		int lzw;
		for (lzw = 95; lzw > 0; lzw--) {
			continue;
		}
	}
	return 60908;
}

int asizdcy::ylnhqzfhuz(string dofhph) {
	string eysvbinkp = "bgmazuvrgvwlhppmfhfybxenytqefoixagrntcizyyvdvsqkpxrwjocqhlnxfgelasbtenectzhypmlcdyuaz";
	string wxniaxkcfkycy = "htterqdcjmuqpidbiptmzjzrltnbbdnnhzidkxvjqytxo";
	if (string("bgmazuvrgvwlhppmfhfybxenytqefoixagrntcizyyvdvsqkpxrwjocqhlnxfgelasbtenectzhypmlcdyuaz") == string("bgmazuvrgvwlhppmfhfybxenytqefoixagrntcizyyvdvsqkpxrwjocqhlnxfgelasbtenectzhypmlcdyuaz")) {
		int slvafmm;
		for (slvafmm = 12; slvafmm > 0; slvafmm--) {
			continue;
		}
	}
	if (string("htterqdcjmuqpidbiptmzjzrltnbbdnnhzidkxvjqytxo") == string("htterqdcjmuqpidbiptmzjzrltnbbdnnhzidkxvjqytxo")) {
		int vwoovmyorr;
		for (vwoovmyorr = 8; vwoovmyorr > 0; vwoovmyorr--) {
			continue;
		}
	}
	return 85651;
}

double asizdcy::qtzshckofktraqtjcdqr(bool vrtrloiexwlt, string lnyhdenjf) {
	double aqymhdj = 25000;
	bool nkvpa = false;
	string xekyccrqt = "mmnkhqnfgeelylpfncrtfbhcbadzcnmpghiujxmlnzirrwvtejytwkeqjcvqjgqvlbvxhlpkadtfhivzeqwagrnccwydn";
	double okliicbsggau = 3058;
	double bagcduokqyazs = 19683;
	string morfjyrrpxpmlhi = "wsplkflifvhvxpjehvxtzfdtga";
	bool kewwiiww = true;
	string nstjyuclrdwxls = "slymypfvwunsbsqrsyupoxkvynedbwriklconmulqjjjjfvjhxj";
	bool mszmrjfblplbap = false;
	if (string("slymypfvwunsbsqrsyupoxkvynedbwriklconmulqjjjjfvjhxj") != string("slymypfvwunsbsqrsyupoxkvynedbwriklconmulqjjjjfvjhxj")) {
		int fluk;
		for (fluk = 10; fluk > 0; fluk--) {
			continue;
		}
	}
	if (false != false) {
		int fp;
		for (fp = 23; fp > 0; fp--) {
			continue;
		}
	}
	if (25000 == 25000) {
		int izysv;
		for (izysv = 68; izysv > 0; izysv--) {
			continue;
		}
	}
	if (false == false) {
		int ipm;
		for (ipm = 64; ipm > 0; ipm--) {
			continue;
		}
	}
	return 65254;
}

int asizdcy::nmjkhlbhdvjxmgzugsrau(string keptkehhwsqwwl) {
	double ebuakajrrobvjy = 53566;
	string yunsrcfvkdsqs = "iidvegziggvgixjopgvbzucrnyvsfffygncrjzeskylmyrqrqozvefshzufdhomlovpvxbtxnixafmongscxokfhiyuygkxgg";
	if (string("iidvegziggvgixjopgvbzucrnyvsfffygncrjzeskylmyrqrqozvefshzufdhomlovpvxbtxnixafmongscxokfhiyuygkxgg") == string("iidvegziggvgixjopgvbzucrnyvsfffygncrjzeskylmyrqrqozvefshzufdhomlovpvxbtxnixafmongscxokfhiyuygkxgg")) {
		int grecud;
		for (grecud = 20; grecud > 0; grecud--) {
			continue;
		}
	}
	return 15418;
}

string asizdcy::rrbklwwkgfccyqhbwhyvy(int vcdtsj, bool agmxqnixliye, double rwbixwaezmhzf, double qkgzwlgk, string avwnjnlfg, bool nwkjkswl, double kuqny, double cxqdapuk, bool xrrezg) {
	int oabbpiz = 9846;
	int jxlsxmvgkch = 9043;
	double pyazbnevinezgeu = 45719;
	return string("zsfsnrajinzolukqkgry");
}

double asizdcy::pdhtmeujjaphhdhpizken(double jvrvmfxwq, int ozcwatkwustr) {
	return 81600;
}

asizdcy::asizdcy() {
	this->nmjkhlbhdvjxmgzugsrau(string("xvfgxiulglrwnjeimtokgsnvsgqkfgvycpkovzcblnrgnqtslsbcyowvoi"));
	this->rrbklwwkgfccyqhbwhyvy(3060, false, 2286, 40681, string("anvhyxzpvkgibxmaxdjttycdnnqfzyjysqa"), false, 375, 19871, true);
	this->pdhtmeujjaphhdhpizken(10586, 1867);
	this->vmbzdclljnclzpy(true, string("omcevffrrtyhalpnnxjdjnusrnwulec"), 2028, string("pfsxhhffvzeqlo"));
	this->zpogkmzzfbhqrcecc(55208, 12768, 6201, string("uuubogeyglljvpejqiiyiqrewmjiofvynkxpwyezpwkwvtywrmqqxevvzeepnpjayadmbniumcuawfkrkgf"), 33304, 62, true, false);
	this->yltfxalvoiothrudl(true);
	this->qefbcqpwhyqb(375, 4100, 1542, 3534, 56981, string("hdvmwsigtwauahacrnrrnvbgcrxaqagaxrziigdqakhmxaoioajjmylymgwdvadhxteoeikkfsplgxylaekkltu"), 5245, 3082, 1227);
	this->iyhydzbrjcrhrladblaodu(1997, true, 2366, 542, false, true, true, 3976);
	this->ztowlcyuhqksk(false);
	this->ylnhqzfhuz(string("rsliozkmzxdhzrpsqqtmqwlghqz"));
	this->qtzshckofktraqtjcdqr(true, string("zpcwaynyn"));
	this->vdycwhrvwdujntrkimrfdnpa();
	this->buhxpredcctuezb(false, 2818, false, 12166, 544, false, false, 5328);
	this->tdsndojxzgqznlyve();
	this->mwhupuuslglbhtbbjqtosik();
	this->wpjkhkhpinz(33820, 25447, false);
	this->oyavjnvjfpryubvjo(32144, true, string("cvwecugdmiqlqkafoiumrzpcezcpazetrhbommxnnxkomqpjsaggccixgrdfmhwtfoqkpbgd"), false, true);
	this->rtaeqhaztubdlwhyneyu(false, 6850, 25355, 48717, string("xufduyitixvlxxgellighswvbglxitxlyhgmlfwziafgwmrsmazuhdpskhthdf"), string("xzphrgdiekslmjrayyoiswqfxglmohupjoxjmgocbdxqtlmbdiazepcbvsppwmmivajcmqtlvhdyhsewdkenzewkudceowiac"), string("ntvnlfkgdwwdguumjnegssyxgmtrqolrixvgdmdbdhrhhauieiruyifzzvcetvkxhjwftnzdlybpfqvolgcyxbqz"), 60470, string("dkxsztbbaswimfbamcwtxxtvsqnxuukocqwochybnwcbwkq"), false);
	this->nzuloijqhr(string("bmzfsnfkbbydnvssesnesmsgujvahkxwxmozxzpnwyysetkyszzkfqwovtoenqeivznrivaguqkptqxiaekfykr"), 3435, 293, true, false, string("urziztzpagpwcprchsvdqooqkazazjyekmigpmkmehzsymo"), 17376);
	this->dkusnujccqwgkohorg(73961, string("lhbieckfuvklkrrzfho"), 1039, 66449, 8457);
	this->lpnmjmhcenudjdetjyop(9037, string("m"), 23650, false);
}

bool ProfileMgr::Load( const std::wstring& path /*= L""*/ )
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;
        if (!acut::file_exists( filepath ))
            return false;

        acut::XmlDoc<wchar_t> xml;
        xml.read_from_file( filepath );

        // Load images in a safe way
        if(xml.has( L"XenosConfig.imagePath" ))
        {
            auto nodes = xml.all_nodes_named( L"XenosConfig.imagePath" );
            for (auto& node : nodes)
                _config.images.emplace_back( node.value() );
        }

        xml.get_if_present( L"XenosConfig.manualMapFlags",  _config.mmapFlags );
        xml.get_if_present( L"XenosConfig.procName",        _config.procName );
        xml.get_if_present( L"XenosConfig.hijack",          _config.hijack );
        xml.get_if_present( L"XenosConfig.unlink",          _config.unlink );
        xml.get_if_present( L"XenosConfig.erasePE",         _config.erasePE );
        xml.get_if_present( L"XenosConfig.close",           _config.close );
        xml.get_if_present( L"XenosConfig.krnHandle",       _config.krnHandle );
        xml.get_if_present( L"XenosConfig.injIndef",        _config.injIndef );
        xml.get_if_present( L"XenosConfig.processMode",     _config.processMode );
        xml.get_if_present( L"XenosConfig.injectMode",      _config.injectMode );
        xml.get_if_present( L"XenosConfig.delay",           _config.delay );
        xml.get_if_present( L"XenosConfig.period",          _config.period );
        xml.get_if_present( L"XenosConfig.skip",            _config.skipProc );
        xml.get_if_present( L"XenosConfig.procCmdLine",     _config.procCmdLine );
        xml.get_if_present( L"XenosConfig.initRoutine",     _config.initRoutine );
        xml.get_if_present( L"XenosConfig.initArgs",        _config.initArgs );

        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}






































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































