//---------------------------------------------------------------------------
#pragma hdrstop
#include "Parametros.h"
#include "VTParametros.h"
#include "VTParametro.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTParametros* NewObjParametros(VTApl *apl)
	{
	return(new TParametros(apl));
	}

//---------------------------------------------------------------------------
__fastcall TParametros::TParametros(VTApl *apl)
	{
   //salva ponteiros p/ objetos
	this->apl = apl;
	//cria listas
	lisARRANJO 		= new TList();
	lisCABO 			= new TList();
	lisCARGA_A 		= new TList();
	lisCARGA_B 		= new TList();
	lisCAPACITOR 	= new TList();
	lisCHAVE       = new TList();
	lisGERADOR 		= new TList();
	lisREGULADOR 	= new TList();
	lisSUPORTE 		= new TList();
	lisSUPRIMENTO 	= new TList();
	lisTRAFOSE 		= new TList();
	lisTRAFOMT 		= new TList();
	lisTRECHOMT		= new TList();
	lisTRECHOBT		= new TList();
	lisTODOS 		= new TList();
	//inicia
	Inicia();
	}

//---------------------------------------------------------------------------
__fastcall TParametros::~TParametros(void)
	{
	//destroi listas e objs
	if(lisARRANJO) 	{LimpaTList(lisARRANJO);    delete lisARRANJO; 		lisARRANJO 		= NULL;}
	if(lisCABO) 		{LimpaTList(lisCABO); 	 	 delete lisCABO; 			lisCABO 			= NULL;}
	if(lisCARGA_A) 	{LimpaTList(lisCARGA_A);    delete lisCARGA_A; 		lisCARGA_A 		= NULL;}
	if(lisCARGA_B) 	{LimpaTList(lisCARGA_B);    delete lisCARGA_B; 		lisCARGA_B 		= NULL;}
	if(lisCAPACITOR) 	{LimpaTList(lisCAPACITOR);  delete lisCAPACITOR; 	lisCAPACITOR 	= NULL;}
	if(lisCHAVE) 		{LimpaTList(lisCHAVE);    	 delete lisCHAVE; 		lisCHAVE 		= NULL;}
	if(lisGERADOR) 	{LimpaTList(lisGERADOR);    delete lisGERADOR; 		lisGERADOR 		= NULL;}
	if(lisREGULADOR) 	{LimpaTList(lisREGULADOR);  delete lisREGULADOR; 	lisREGULADOR 	= NULL;}
	if(lisSUPORTE) 	{LimpaTList(lisSUPORTE); 	 delete lisSUPORTE; 		lisSUPORTE 		= NULL;}
	if(lisSUPRIMENTO) {LimpaTList(lisSUPRIMENTO); delete lisSUPRIMENTO; 	lisSUPRIMENTO 	= NULL;}
	if(lisTRAFOSE) 	{LimpaTList(lisTRAFOSE); 	 delete lisTRAFOSE; 		lisTRAFOSE 		= NULL;}
	if(lisTRECHOMT) 	{LimpaTList(lisTRECHOMT);	 delete lisTRECHOMT; 	lisTRECHOMT 	= NULL;}
	if(lisTRECHOBT) 	{LimpaTList(lisTRECHOBT);	 delete lisTRECHOBT; 	lisTRECHOBT 	= NULL;}

	//só destroi lista
	if(lisTODOS) 		{delete lisTODOS; 		lisTODOS = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TParametros::Atualiza(int tipo, AnsiString atributo, double valor)
	{
	//variáveis locais
	VTParametro  *par;
	TList 		 *lisTIPO = LisTipo(tipo);

	try{
		for(int np = 0; np < lisTIPO->Count; np++)
			{//localiza parametro pelo tipo do eqpto e atributo
			par = (VTParametro*)lisTIPO->Items[np];
			if(par->TipoEqpto != tipo) continue;
			if(atributo.AnsiCompareIC(par->Atributo) == 0)
				{//atualiza valor do parâmetro
				par->Valor = valor;
				}
			}
		}catch(...) {return(false);}
	return(true);
	}

//---------------------------------------------------------------------------
VTParametro* __fastcall TParametros::Busca(int tipo, AnsiString atributo)
	{
	//variáveis locais
	VTParametro  *par;
	TList			 *lisTipo = LisTipo(tipo);

	for(int np = 0; np < lisTipo->Count; np++)
		{//localiza na lista o parametro com mesmo atributo
		par = (VTParametro*)lisTipo->Items[np];
		if(par->TipoEqpto != tipo) continue;
		if(par->Atributo.AnsiCompareIC(atributo) == 0)
			return(par);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
bool __fastcall  TParametros::Busca(int tipo, TList *lisPAR)
	{
	//variáveis locais
	TList *lisTIPO = LisTipo(tipo);

	//copia objs para lista
	if(lisTIPO) CopiaTList(lisTIPO, lisPAR);
	else return(false);
	return(true);
	}

//---------------------------------------------------------------------------
VTParametro* __fastcall TParametros::CriaPar(int tipo, AnsiString atributo,
														AnsiString formato, double valor)
	{
	//variáveis locais
	VTParametro  *par;
	TList			 *lisTipo = LisTipo(tipo);

	try{//cria obj parametro e insere na lista
		par = NewObjParametro(apl);
		par->TipoEqpto = tipo;
		par->Atributo = atributo;
		par->Formato = formato;
		par->Valor = valor;
		//lê do infoset o valor salvo (se houver)
		LeValorSalvo(par);
		lisTipo->Add(par);
		//insere opções de valor (se houver)
		InsereOpcoes(par);
	}catch(...)
		{
		return(NULL);
		}
	return(par);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::Inicia(void)
	{
	//inicia cada lista
	IniciaArranjo();
	IniciaCabo();
	IniciaCargaA();
	IniciaCargaB();
	IniciaCapacitor();
	IniciaChave();
	IniciaGerador();
	IniciaRegulador();
	IniciaSuporte();
	IniciaSuprimento();
	IniciaTrafoSE();
	IniciaTrafoMT();
	IniciaTrechoMT();
	IniciaTrechoBT();
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaArranjo(void)
	{
	//variáveis locais
	int tipo = tipoARRANJO;

	//cria objs parametros
	CriaPar(tipo,"I adm.[A]","%2.1f",400.);
	CriaPar(tipo,"R0",		 "%4.3f",0.01);
	CriaPar(tipo,"X0",		 "%4.3f",0.05);
	CriaPar(tipo,"R1",		 "%4.3f",0.01);
	CriaPar(tipo,"X1",		 "%4.3f",0.05);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaCabo(void)
	{
	//variáveis locais
	int tipo = tipoCABO;

	//cria objs parametros
	CriaPar(tipo,"I adm.[A]",				"%2.1f",400.0);
	CriaPar(tipo,"Resistência [ohm/km]","%4.3f",2.000);
	CriaPar(tipo,"Raio[m]",					"%4.3f",0.020);
	CriaPar(tipo,"RMG[m]",					"%4.3f",0.001);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaCargaA(void)
	{
	//variáveis locais
	int 			  tipo = tipoCARGA_A;

	//cria objs parametros
	CriaPar(tipo,"Consumo Máx.[kWh/mês]",		 "%2.1f",5000.0);
	CriaPar(tipo,"Dem. Contratada Máx.[kW]",   "%2.1f", 100.0);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaCargaB(void)
	{
	//variáveis locais
	int tipo = tipoCARGA_B;

	//cria objs parametros
	CriaPar(tipo,"Consumo Máx.[kWh/mês]",		 "%2.1f",5000.0);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaCapacitor(void)
	{
	//variáveis locais
	int tipo = tipoCAPACITOR;

	//cria objs parametros
	CriaPar(tipo,"Q [kvar]",			"%2.1f",300.0);
	CriaPar(tipo,"Ligação",				"%1.0f",1	 );
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaChave(void)
	{
	//variáveis locais
	int tipo = tipoCHAVE;

	//cria objs parametros
	CriaPar(tipo,"I adm.[A]",				"%2.1f",400.0);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaGerador(void)
	{
	//variáveis locais
	int tipo = tipoGERADOR;

	//cria objs parametros
	CriaPar(tipo,"Snom [kVA]", 			"%2.1f",200.);
	CriaPar(tipo,"Tensão limite [pu]",	"%3.2f",1.10);
	CriaPar(tipo,"Ligação",					"%1.0f",1	);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaRegulador(void)
	{
	//variáveis locais
	int tipo = tipoREGULADOR;

	//cria objs parametros
	CriaPar(tipo,"Snom unid. monof.[kVA]",     "%2.1f",200.);
	CriaPar(tipo,"Var. tensão [%]",				 "%2.0f",10	 );
	CriaPar(tipo,"Qtd. passos",					 "%2.0f",16  );
	CriaPar(tipo,"Tensão alvo [pu]",				 "%3.2f",1.10);
	CriaPar(tipo,"Ligação",						    "%1.0f",12  );
	CriaPar(tipo,"Ajuste",							 "%1.0f",1   );
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaSuporte(void)
	{
	//variáveis locais
	int tipo = tipoSUPORTE;

	//cria objs parametros
	CriaPar(tipo,"Coord. X ponto 1 [m]","%4.3f",0.22);
	CriaPar(tipo,"Coord. X ponto 2 [m]","%4.3f",0.92);
	CriaPar(tipo,"Coord. X ponto 3 [m]","%4.3f",1.66);
	CriaPar(tipo,"Coord. X ponto 4 [m]","%4.3f",0.00);
	CriaPar(tipo,"Coord. Y ponto 1 [m]","%4.3f",10.0);
	CriaPar(tipo,"Coord. Y ponto 2 [m]","%4.3f",10.0);
	CriaPar(tipo,"Coord. Y ponto 3 [m]","%4.3f",10.0);
	CriaPar(tipo,"Coord. Y ponto 4 [m]","%4.3f",7.20);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaSuprimento(void)
	{
	//variáveis locais
	int tipo = tipoSUPRIMENTO;

	//cria objs parametros
	CriaPar(tipo,"Vnom [kV]",			   "%2.1f",69.  );
	CriaPar(tipo,"R0 equivalente [pu]", "%4.3f",0.01 );
	CriaPar(tipo,"X0 equivalente [pu]", "%4.3f",0.05 );
	CriaPar(tipo,"R1 equivalente [pu]", "%4.3f",0.01 );
	CriaPar(tipo,"X1 equivalente [pu]", "%4.3f",0.05 );
	CriaPar(tipo,"Pcc fase-terra [MW]", "%2.1f",0.0  );
	CriaPar(tipo,"Qcc fase-terra [MW]", "%2.1f",2000.);
	CriaPar(tipo,"Pcc trifásico [Mvar]","%2.1f",0.0  );
	CriaPar(tipo,"Qcc trifásico [Mvar]","%2.1f",2000.);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaTrafoSE(void)
	{
	//variáveis locais
	int tipo = tipoTRAFOSE;

	//cria objs parametros
	CriaPar(tipo,"Snom [MVA]", 			"%2.1f",10. );
	CriaPar(tipo,"Vnom primário [kV]",  "%3.2f",69. );
	CriaPar(tipo,"Vnom secundário [kV]","%3.2f",13.8);
	CriaPar(tipo,"Ligação primário",		"%1.0f",1	);
	CriaPar(tipo,"Ligação secundário",	"%1.0f",0	);
	CriaPar(tipo,"Perda Fe [%]",			"%2.1f",0.1 );
	CriaPar(tipo,"Perda Tot.[kW]",		"%2.1f",10. );
	CriaPar(tipo,"R0 [pu]",					"%4.3f",0.01);
	CriaPar(tipo,"X0 [pu]",					"%4.3f",0.05);
	CriaPar(tipo,"R1 [pu]",					"%4.3f",0.01);
	CriaPar(tipo,"X1 [pu]",					"%4.3f",0.05);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaTrafoMT(void)
	{
	//variáveis locais
	int tipo = tipoTRAFOMT;

	//cria objs parametros
	CriaPar(tipo,"Snom [kVA]", 			"%2.1f",100.);
	CriaPar(tipo,"Vnom primário [kV]",  "%3.2f",13.8);
	CriaPar(tipo,"Vnom secundário [kV]","%3.2f",0.38);
	CriaPar(tipo,"Ligação primário",		"%1.0f",1	);
	CriaPar(tipo,"Ligação secundário",	"%1.0f",0	);
	CriaPar(tipo,"Perda Fe [%]",			"%2.1f",0.1 );
	CriaPar(tipo,"Perda Tot.[kW]",		"%2.1f",10. );
	CriaPar(tipo,"R0 [pu]",					"%4.3f",0.01);
	CriaPar(tipo,"X0 [pu]",					"%4.3f",0.05);
	CriaPar(tipo,"R1 [pu]",					"%4.3f",0.01);
	CriaPar(tipo,"X1 [pu]",					"%4.3f",0.05);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaTrechoMT(void)
	{
	//variáveis locais
	int tipo = tipoTRECHOMT;

	//cria objs parametros
	CriaPar(tipo,"Cabo Fase",   				"%1.0f",0   );
	CriaPar(tipo,"Cabo Neutro", 				"%1.0f",0   );
	CriaPar(tipo,"Compr. Máx. do Vão [m]", "%2.1f",200.);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::IniciaTrechoBT(void)
	{
	//variáveis locais
	int tipo = tipoTRECHOBT;

	//cria objs parametros
	CriaPar(tipo,"Cabo Fase",   				"%1.0f",0   );
	CriaPar(tipo,"Cabo Neutro", 				"%1.0f",0   );
	CriaPar(tipo,"Compr. Máx. do Vão [m]", "%2.1f",200.);
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::InsereOpcoes(VTParametro *par)
	{
	//variáveis locais
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCabo	  *cabo;

	//verifica se é um parâmetro especial (com opções de valor)
	switch(par->TipoEqpto)
		{
		case tipoCAPACITOR:
			if(par->Atributo.AnsiCompareIC("Ligação") == 0)
				{
				par->Opcoes->Add("Estrela aterrado");
				par->Opcoes->Add("Delta");
				par->Opcoes->Add("Estrela");
				break;
				}
		case tipoGERADOR:
			if(par->Atributo.AnsiCompareIC("Ligação") == 0)
				{
				par->Opcoes->Add("Estrela aterrado");
				par->Opcoes->Add("Delta");
				par->Opcoes->Add("Estrela");
				break;
				}
		case tipoREGULADOR:
			if(par->Atributo.AnsiCompareIC("Ligação") == 0)
				{
				par->Opcoes->Add("1 unid. Fases AB");
				par->Opcoes->Add("1 unid. Fases BC");
				par->Opcoes->Add("1 unid. Fases CA");
				par->Opcoes->Add("1 unid. Fases AN");
				par->Opcoes->Add("1 unid. Fases BN");
				par->Opcoes->Add("1 unid. Fases CN");
				par->Opcoes->Add("1 unid. Fases ABN");
				par->Opcoes->Add("1 unid. Fases BCN");
				par->Opcoes->Add("1 unid. Fases CAN");
				par->Opcoes->Add("2 unid. Delta Aberto(CA,AB)");
				par->Opcoes->Add("2 unid. Delta Aberto(AB,BC)");
				par->Opcoes->Add("2 unid. Delta Aberto(BC,CA)");
				par->Opcoes->Add("3 unid. Delta Fechado");
				par->Opcoes->Add("3 unid. Estrela");
				break;
				}
			if(par->Atributo.AnsiCompareIC("Ajuste") == 0)
				{
				par->Opcoes->Add("Automático");
				par->Opcoes->Add("Tap fixo");
				break;
				}
		case tipoTRAFOSE:
			if(par->Atributo.AnsiPos("Ligação") > 0)
				{
				par->Opcoes->Add("Estrela aterrado");
				par->Opcoes->Add("Delta");
				par->Opcoes->Add("Estrela");
				break;
				}
		case tipoTRAFOMT:
			if(par->Atributo.AnsiPos("Ligação") > 0)
				{
				par->Opcoes->Add("Estrela aterrado");
				par->Opcoes->Add("Delta");
				par->Opcoes->Add("Estrela");
				break;
				}
		case tipoTRECHOMT:
			if(par->Atributo.AnsiPos("Cabo Fase") > 0)
				{
				for(int nc = 0; nc < arranjos->LisCabo()->Count; nc++)
					{
					cabo = (VTCabo*)arranjos->LisCabo()->Items[nc];
					par->Opcoes->Add(cabo->Codigo);
					}
				break;
				}
			if(par->Atributo.AnsiPos("Cabo Neutro") > 0)
				{
				for(int nc = 0; nc < arranjos->LisCabo()->Count; nc++)
					{
					cabo = (VTCabo*)arranjos->LisCabo()->Items[nc];
					par->Opcoes->Add(cabo->Codigo);
					}
				break;
				}
		case tipoTRECHOBT:
			if(par->Atributo.AnsiPos("Cabo Fase") > 0)
				{
				for(int nc = 0; nc < arranjos->LisCabo()->Count; nc++)
					{
					cabo = (VTCabo*)arranjos->LisCabo()->Items[nc];
					par->Opcoes->Add(cabo->Codigo);
					}
				break;
				}
			if(par->Atributo.AnsiPos("Cabo Neutro") > 0)
				{
				for(int nc = 0; nc < arranjos->LisCabo()->Count; nc++)
					{
					cabo = (VTCabo*)arranjos->LisCabo()->Items[nc];
					par->Opcoes->Add(cabo->Codigo);
					}
				break;
				}
		default: break;
      }
	}

//---------------------------------------------------------------------------
void __fastcall TParametros::LeValorSalvo(VTParametro *par)
	{
	//variáveis locais
	VTGeral    *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString valor;

	if(! geral) return;
	if(! par) return;
	if(! geral->Infoset->LoadFromFile("ParamImporta")) return;
	if(! geral->Infoset->GetInfo(par->TipoStr + par->Atributo, valor)) return;
	par->Valor = StrToDouble(valor);
	}

//---------------------------------------------------------------------------
TList*   __fastcall TParametros::LisTipo(int tipo)
	{
	switch(tipo)
		{
		case tipoARRANJO:    return(lisARRANJO);
		case tipoCABO:       return(lisCABO);
		case tipoCAPACITOR:  return(lisCAPACITOR);
		case tipoCARGA_A:    return(lisCARGA_A);
		case tipoCARGA_B:    return(lisCARGA_B);
		case tipoCHAVE:      return(lisCHAVE);
		case tipoGERADOR:    return(lisGERADOR);
		case tipoREGULADOR:  return(lisREGULADOR);
		case tipoSUPORTE:    return(lisSUPORTE);
		case tipoSUPRIMENTO: return(lisSUPRIMENTO);
		case tipoTRAFOSE:    return(lisTRAFOSE);
		case tipoTRAFOMT: 	return(lisTRAFOMT);
		case tipoTRECHOMT:   return(lisTRECHOMT);
		case tipoTRECHOBT:   return(lisTRECHOBT);
		default:					return(NULL);
		}
	}

//---------------------------------------------------------------------------
TList*   __fastcall TParametros::PM_LisParametros(void)
	{
	//limpa lista com todos os parâmetros
	lisTODOS->Clear();
	//copia objs de cada lista
	CopiaTList(lisARRANJO, 		lisTODOS);
	CopiaTList(lisCABO, 			lisTODOS);
	CopiaTList(lisCARGA_A, 		lisTODOS);
	CopiaTList(lisCARGA_B, 		lisTODOS);
	CopiaTList(lisCAPACITOR,	lisTODOS);
	CopiaTList(lisGERADOR, 		lisTODOS);
	CopiaTList(lisREGULADOR,	lisTODOS);
	CopiaTList(lisSUPORTE, 		lisTODOS);
	CopiaTList(lisSUPRIMENTO, 	lisTODOS);
	CopiaTList(lisTRAFOSE, 		lisTODOS);
	CopiaTList(lisTRAFOMT, 		lisTODOS);
	CopiaTList(lisTRECHOMT, 	lisTODOS);
	CopiaTList(lisTRECHOBT, 	lisTODOS);
	return(lisTODOS);
	}

//---------------------------------------------------------------------------
bool __fastcall TParametros::Valor(int tipo, AnsiString atributo, double &valor)
	{
	//variáveis locais
	VTParametro  *par;
	TList 		 *lisTIPO = LisTipo(tipo);

	try{
		for(int np = 0; np < lisTIPO->Count; np++)
			{//localiza parametro pelo tipo do eqpto e atributo
			par = (VTParametro*)lisTIPO->Items[np];
			if(par->TipoEqpto != tipo) continue;
			if(atributo.AnsiCompareIC(par->Atributo) == 0)
				{//retorna valor do parâmetro
				valor = par->Valor;
				return(true);
				}
			}
		}catch(...) {return(false);}
	return(false);
	}

//---------------------------------------------------------------------------
