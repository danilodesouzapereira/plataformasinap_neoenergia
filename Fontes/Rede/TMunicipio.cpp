// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMunicipio.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTMunicipio
// ---------------------------------------------------------------------------
VTMunicipio* __fastcall NewObjMunicipio(void)
{
	return (new TMunicipio());
}

// ---------------------------------------------------------------------------
__fastcall TMunicipio::TMunicipio(void)
{
	// cria slisNome
	slisNome = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TMunicipio::~TMunicipio(void)
{
	// destroi
	if (slisNome)
	{
		delete slisNome;
		slisNome = NULL;
	}
}

// ---------------------------------------------------------------------------
int __fastcall TMunicipio::Codigo(AnsiString nome)
{
	//localiza chave pelo valor
	int id = 0;
	std::map<AnsiString, int>::iterator it = municipio.begin();

	it = municipio.find(nome);

	if(it != municipio.end())
	{
		id = it->second;
	}
    return id;
}

// ---------------------------------------------------------------------------
bool __fastcall TMunicipio::Insere(int codigo, AnsiString nome, AnsiString regiao)
{
//	// verifica se existe no map ou slisNome
//	int cod_existente = Codigo(nome);
//	if (codigo == cod_existente)
//		return false;
//	if (slisNome->IndexOf(nome) == 0)
//		return false;
//	// insere chave/valor no map
//	municipio[codigo] = nome;
//	// insere no slisNome
//	slisNome->Add(nome);
//	return true;

	// verifica se existe no map ou slisNome
	int cod_existente = Codigo(nome);
	if (codigo == cod_existente)
		return false;
	else
	{
		municipio[nome] = codigo;
        mun_regiao[codigo] = regiao;
		// insere no slisNome
		slisNome->Add(nome);
    }
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TMunicipio::Limpa(void)
{
	// limpa map e slisNome
	municipio.clear();
    mun_regiao.clear();
	slisNome->Clear();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TMunicipio::Nome(int codigo)
{
    //localiza chave pelo valor
	AnsiString nome = "";

	std::map<AnsiString,int>::iterator it = municipio.begin();
	while(it != municipio.end())
	{
		if(it->second == codigo)
			return(it->first);
		it++;
	}

	return ("");
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15257212
//como na tabela fornecida
AnsiString __fastcall TMunicipio::Regiao(int codigo)
{
    AnsiString reg = "";
	//std::map<int, AnsiString>::iterator it = mun_regiao.begin();

    //it = mun_regiao.find(codigo);
    if(mun_regiao.find(codigo) != mun_regiao.end())
	{
		reg = mun_regiao.find(codigo)->second;
	}

	return (reg);
    /*
	AnsiString siglaRegiao ="XX";

	switch (codigo)
	{
		case 4103040:
		case 4103958:
		case 4107736:
		case 4108957:
		case 4109401:
		case 4110102:
		case 4110201:
		case 4110706:
		case 4111407:
		case 4113254:
		case 4117800:
		case 4119301:
		case 4119608:
		case 4120606:
		case 4121505:
		case 4122008:
		case 4123857:
		case 4127007:
		case 4127965:
			siglaRegiao = "CI";
			break;
		case 4101606:
		case 4104659:
		case 4104907:
		case 4107009:
		case 4107751:
		case 4110078:
		case 4110508:
		case 4112009:
		case 4117305:
		case 4117701:
		case 4119400:
		case 4119905:
		case 4120101:
		case 4121703:
		case 4126207:
		case 4126306:
		case 4127106:
		case 4127502:
		case 4128534:
			siglaRegiao = "CP";
			break;
		case 4101309:
		case 4102901:
		case 4106803:
		case 4108502:
		case 4113908:
		case 4118600:
		case 4118709:
		case 4120309:
		case 4125100:
		case 4125605:
		case 4128203:
		case 4213609:
            siglaRegiao = "CU";
			break;
		case 4106902:
			siglaRegiao = "LC";
			break;
		case 4101200:
		case 4109500:
		case 4109609:
		case 4115705:
		case 4116208:
		case 4118204:
		case 4119954:
			siglaRegiao = "LI";
			break;
		case 4100202:
		case 4100400:
		case 4103107:
		case 4104006:
		case 4104253:
		case 4105201:
		case 4105805:
		case 4111258:
		case 4119152:
		case 4119509:
		case 4120804:
		case 4122206:
		case 4127882:
		case 4128633:
			siglaRegiao = "LN";
			break;
		case 4100301:
		case 4101804:
		case 4102307:
		case 4104105:
		case 4104204:
		case 4106209:
		case 4107652:
		case 4113205:
		case 4114302:
		case 4119103:
		case 4121208:
		case 4122305:
		case 4125506:
		case 4127601:
			siglaRegiao = "LS";
			break;
		case 4100459:
		case 4101705:
		case 4102505:
		case 4103008:
		case 4103909:
		case 4104303:
		case 4106555:
		case 4107504:
		case 4107553:
		case 4107702:
		case 4108601:
		case 4110805:
		case 4112207:
		case 4112959:
		case 4113734:
		case 4114005:
		case 4115101:
		case 4115739:
		case 4116109:
		case 4116802:
		case 4117271:
		case 4118808:
		case 4120655:
		case 4121109:
		case 4121356:
		case 4122503:
		case 4128005:
			siglaRegiao = "NC";
			break;
		case 4107306:
		case 4107900:
		case 4111100:
		case 4111605:
		case 4114104:
		case 4114203:
		case 4114807:
		case 4115200:
		case 4117404:
		case 4117503:
		case 4120408:
		case 4125308:
		case 4126256:
			siglaRegiao = "NM";
			break;
		case 4100608:
		case 4100905:
		case 4102208:
		case 4105904:
		case 4106704:
		case 4107108:
		case 4107801:
		case 4108908:
		case 4110300:
		case 4110904:
		case 4111308:
		case 4112603:
		case 4113502:
		case 4115002:
		case 4115903:
		case 4116505:
		case 4116901:
		case 4117107:
		case 4118006:
		case 4118105:
		case 4118303:
		case 4118402:
		case 4119707:
		case 4120200:
		case 4121000:
		case 4123303:
		case 4123600:
		case 4123709:
		case 4123956:
		case 4124202:
		case 4124608:
		case 4124905:
		case 4125902:
		case 4126702:
		case 4127304:
		case 4128302:
			siglaRegiao = "NP";
			break;
		case 4100509:
		case 4100707:
		case 4103370:
		case 4103479:
		case 4105508:
		case 4105607:
		case 4106605:
		case 4107256:
		case 4107520:
		case 4108320:
		case 4109104:
		case 4109906:
		case 4110409:
		case 4110607:
		case 4111555:
		case 4112405:
		case 4113007:
		case 4114708:
		case 4117206:
		case 4118857:
		case 4118907:
		case 4122602:
		case 4125357:
		case 4125555:
		case 4126108:
		case 4126801:
		case 4126900:
		case 4127205:
		case 4127908:
		case 4128104:
		case 4128625:
		case 4128807:
			siglaRegiao = "NU";
			break;
		case 4101002:
		case 4102604:
		case 4102752:
		case 4103024:
		case 4103156:
		case 4104501:
		case 4106571:
		case 4107207:
		case 4107405:
		case 4107850:
		case 4108403:
		case 4114351:
		case 4115408:
		case 4116950:
		case 4117255:
		case 4119004:
		case 4119251:
		case 4119806:
		case 4120358:
		case 4121406:
		case 4121604:
		case 4122800:
		case 4123006:
		case 4123808:
		case 4124400:
		case 4125209:
		case 4128609:
			siglaRegiao = "OB";
			break;
		case 4101051:
		case 4103057:
		case 4103354:
		case 4103453:
		case 4104055:
		case 4104600:
		case 4104808:
		case 4105003:
		case 4106308:
		case 4107124:
		case 4109302:
		case 4109757:
		case 4110052:
		case 4113452:
		case 4116703:
		case 4123824:
		case 4124020:
		case 4127858:
			siglaRegiao = "OC";
			break;
		case 4105300:
		case 4107157:
		case 4108304:
		case 4110953:
		case 4115606:
		case 4115804:
		case 4116059:
		case 4121257:
		case 4123501:
		case 4124053:
		case 4125456:
		case 4125704:
		case 4125753:
		case 4126355:
		case 4128559:
			siglaRegiao = "OF";
			break;
		case 4103222:
		case 4104428:
		case 4104451:
		case 4105409:
		case 4105706:
		case 4106456:
		case 4106506:
		case 4107546:
		case 4108452:
		case 4108650:
		case 4109658:
		case 4111209:
		case 4113304:
		case 4114401:
		case 4115309:
		case 4115457:
		case 4117057:
		case 4117602:
		case 4118501:
		case 4120150:
		case 4120903:
		case 4121752:
		case 4122156:
		case 4124806:
		case 4126272:
		case 4126652:
		case 4128658:
		case 4128708:
			siglaRegiao = "OP";
			break;
		case 4102000:
		case 4107538:
		case 4108205:
		case 4108809:
		case 4110656:
		case 4112751:
		case 4114609:
		case 4115358:
		case 4115853:
		case 4117222:
		case 4117453:
		case 4117909:
		case 4118451:
		case 4120853:
		case 4127403:
		case 4127700:
		case 4127957:
			siglaRegiao = "OT";
			break;
		case 4101150:
		case 4101408:
		case 4101507:
		case 4101655:
		case 4101853:
		case 4102109:
		case 4103206:
		case 4103305:
		case 4103503:
		case 4103800:
		case 4104402:
		case 4106852:
		case 4107603:
		case 4108106:
		case 4108551:
		case 4108700:
		case 4110003:
		case 4111506:
		case 4112108:
		case 4112504:
		case 4113106:
		case 4113429:
		case 4113601:
		case 4113759:
		case 4114500:
		case 4114906:
		case 4115507:
		case 4115754:
		case 4116307:
		case 4117297:
		case 4119657:
		case 4122107:
		case 4122172:
		case 4122651:
		case 4122701:
		case 4123402:
		case 4125001:
		case 4125803:
			siglaRegiao = "TA";
			break;
		case 4100103:
		case 4101101:
		case 4101903:
		case 4102406:
		case 4103602:
		case 4104709:
		case 4106001:
		case 4106100:
		case 4106407:
		case 4109005:
		case 4109708:
		case 4111001:
		case 4111704:
		case 4112306:
		case 4112801:
		case 4112900:
		case 4113403:
		case 4116604:
		case 4117008:
		case 4117214:
		case 4119202:
		case 4120705:
		case 4121307:
		case 4121901:
		case 4122909:
		case 4123105:
		case 4123204:
		case 4123907:
		case 4124004:
		case 4124103:
		case 4124301:
		case 4124707:
		case 4125407:
		case 4126009:
		case 4126405:
		case 4126603:
		case 4127809:
		case 4128401:
		case 4128500:
			siglaRegiao = "TC";
			break;
		case 4100806:
		case 4102802:
		case 4103404:
		case 4103701:
		case 4105102:
		case 4108007:
		case 4109203:
		case 4109807:
		case 4111902:
		case 4112702:
		case 4113700:
		case 4113809:
		case 4116000:
		case 4116406:
		case 4120002:
		case 4120333:
		case 4120507:
		case 4122404:
		case 4124509:
		case 4126504:
		case 4126678:
			siglaRegiao = "TL";
			break;
    default:
		;
	}

	return siglaRegiao;
    */
}
// ---------------------------------------------------------------------------
TStringList* __fastcall TMunicipio::SLisNome(void)
{
    return slisNome;
}

// ---------------------------------------------------------------------------
// eof
