// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "TEmpresa.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
VTEmpresa* __fastcall NewObjEmpresa(void)
{
	return (new TEmpresa());
}

// ---------------------------------------------------------------------------
__fastcall TEmpresa::TEmpresa(void)
{
	// inicia map
	MontaMap();
}

// ---------------------------------------------------------------------------
__fastcall TEmpresa::~TEmpresa(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
UnicodeString __fastcall TEmpresa::EmpresaAsStr(int empresa_id)
{
	//variaveis locais
	UnicodeString descricao = "";

	if(empresa.find(empresa_id) != empresa.end())
	{//localiza nome da empresa
		descricao = empresa.find(empresa_id)->second;
    }
	return descricao;
}

// ---------------------------------------------------------------------------
int __fastcall TEmpresa::EmpresaAsInt(UnicodeString empresa_nome)
{
	//variaveis locais
	int id = 0;

	std::map<int, UnicodeString>::iterator it = empresa.begin();
	while(it != empresa.end())
    {// localiza id da empresa
		if(it->second.CompareIC(empresa_nome) == 0)
			return it->first;
        it++;
	}
	return id;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18194486
// https://sinapsisenergia.teamwork.com/#/tasks/17674033
void __fastcall TEmpresa::MontaMap(void)
{
	empresa[SINAPSIS] = "SINAPSIS";
	empresa[ENERQ] = "ENERQ";
	empresa[ELETROPAULO] = "ENEL-SP";//"ELETROPAULO";
	empresa[MACKENZIE] = "MACKENZIE";
	empresa[UFABC] = "UFABC";
	empresa[UFBA] = "UFBA";
	empresa[COPEL] = "COPEL";
	empresa[RGESUL] = "RGESUL";
	empresa[UNISINOS] = "UNISINOS";
	empresa[GRUPOREDE] = "GRUPOREDE";
	empresa[CEEE] = "CEEE";
	empresa[FECOERGS] = "FECOERGS";
	//empresa[CEAL] = "CEAL2";
	empresa[CEMIG] = "CEMIG";
	empresa[USP] = "EPUSP";
	empresa[CEB] = "CEB";
	empresa[CERTAJA] = "CERTAJA";
	empresa[CERTEL] = "CERTEL";
	empresa[COPREL] = "COPREL";
	empresa[CRELUZ] = "CRELUZ";
	empresa[CERMISSOES] = "CERMISSOES";
	empresa[CERILUZ] = "CERILUZ";
	empresa[COOPERLUZ] = "COOPERLUZ";
	empresa[CRERAL] = "CRERAL";
	empresa[EDP_BANDEIRANTES] = "EDPSP";
	empresa[EDP_ESCELSA] = "EDPES";
	empresa[CEPEL] = "CEPEL";
	empresa[ELETROBRAS_HOLDING] = "ELETROBRAS";
	empresa[URUGUAI] = "URUGUAI";
	empresa[EDP] = "EDP";
	empresa[ELETROBRAS_ACRE] = "EDAC";
	empresa[ELETROBRAS_ALAGOAS] = "CEAL";
	empresa[ELETROBRAS_AMAZONAS] = "EDAM";
	empresa[ELETROBRAS_PIAUI] = "EDPI";
	empresa[ELETROBRAS_RONDONIA] = "EDRO";
	empresa[ELETROBRAS_RORAIMA] = "EDRR";
	empresa[UNIJORGE] = "UNIJORGE";
	empresa[REINO_UNIDO] = "REINOUNIDO";
	empresa[COOPER] = "COOPER";
	empresa[IFES] = "IFES";
	empresa[UFJF] = "UFJF";
	empresa[CEJAMA] = "CEJAMA";
	empresa[CERGAPA] = "CERGAPA";
	empresa[CERBRANORTE] = "CERBRANORTE";
	empresa[CERGRAL] = "CERGRAL";
	empresa[COOPERMILA] = "COOPERMILA";
	empresa[CERPALO] = "CERPALO";
	empresa[COOPERALIANCA] = "COOPERALIANCA";
	empresa[CEPRAG] = "CEPRAG";
	empresa[CERSUL] = "CERSUL";
	empresa[CERMOFUL] = "CERMOFUL";
	empresa[COOPERCOCAL] = "COOPERCOCAL";
	empresa[CEREJ] = "CEREJ";
	empresa[CERGAL] = "CERGAL";
	empresa[COORSEL] = "COORSEL";
	empresa[CERAL_ANITAPOLIS] = "CERAL";
	empresa[CERACA] = "CERACA";
	empresa[FECOERUSC] = "FECOERUSC";
	empresa[CEGERO] = "CEGERO";
	empresa[CERSAD] = "CERSAD";
	empresa[CEESAM] = "CEESAM";
	empresa[COOPERZEM] = "COOPERZEM";
	empresa[IFSP] = "IFSP";
	empresa[UFES] = "UFES";
	empresa[CERTHIL] = "CERTHIL";
	empresa[CERTREL] = "CERTREL";
	empresa[UNIVATES] = "UNIVATES";
	empresa[COELBA] = "COELBA";  //71
	empresa[CELPE] = "CELPE";    //72
	empresa[COSERN] = "COSERN";  //73
	empresa[UNIFEI] = "UNIFEI";  //74
	empresa[COOPERNORTE] = "COOPERNORTE";  //75
	empresa[USF] = "USF";     //76
	empresa[CPFL] = "CPFL";   //79
	empresa[LIGHT] = "LIGHT";  //83
    empresa[CERFOX] = "CERFOX"; // 85 nova empresa
}

// -----------------------------------------------------------------------------
// eof
