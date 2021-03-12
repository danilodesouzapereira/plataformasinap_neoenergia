//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TCalcIndices.h"
//#include "..\GeraCorte\TCorte.h"
#include "..\GeraCorte\TCorteConf.h"
#include "..\VTConfiab.h"
#include "..\VTDadoConfiab.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTCarga.h"
#include "..\Classes\VTClasse.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrecho.h"
#include "..\Constante\Const.h"
#include "..\Diretorio\VTPath.h"
#include "..\Diretorio\DirArq.h"
#include "..\GeraPath\VTArvores.h"
#include "..\GeraRede\TLigaSerie.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTCalcIndCont* __fastcall NewObjCalcIndice(VTApl *apl)
   {
   return(new TCalcIndices(apl));
   }
//---------------------------------------------------------------------------
__fastcall TCalcIndices::TCalcIndices(VTApl* apl)
   {
   this->apl = apl;
	}
//---------------------------------------------------------------------------
__fastcall TCalcIndices::~TCalcIndices(void)
	{
	}
//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::Executa(TList *lisCortes, TList *lisArvs, VTProgresso *prog)
	{
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	VTEqpto *eqpto1, *eqpto2, *eqpto3;
	VTArvores *arvs;
	TCorteConf   *pcorte1, *pcorte2, *pcorte3;
	double lambda_f1, lambda_f2, lambda_f3, lambda_m1, lambda_m2, lambda_m3;
	double tx_ff, r_ff, tx_fm, r_fm, tx_fff, r_fff, tx_ffm, r_ffm;
	double ni12, ni13, ni31, ni32, ni23, ni21;
	double a123, a312, a231, w123, w312, w231;
	double dic, fic;
	double  p1tr, p1tm, p2tr, p2tm, p3tr, p3tm;
	double  p1tf, p1ti, p2tf, p2ti, p3tf, p3ti;
	double  dc1, dc2, dc3, dcomp;
	PAR_CONF p1, p2, p3;
	TList* lisPath;
	//RES_MC res;

	VTPath* path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq = path->DirTmp() + "\\calInd.tmp";
	FILE *fp = NULL; fp = fopen(arq.c_str(), "wt");


	//Inicia indicadores por arvore
	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		arvs->res_conf.dic = 0.;
		arvs->res_conf.fic = 0.;
		arvs->res_conf.tx_ff = 0.;
		arvs->res_conf.tx_fm = 0.;
		arvs->res_conf.r_ff  = 0.;
		arvs->res_conf.r_fm  = 0.;
		}

	if(prog) prog->Start(progTEXTO);
	if(prog) prog->Add("Cálculo de indicadores");
	for(int nc = 0; nc < lisCortes->Count; nc++)
		{
		pcorte1 = (TCorteConf*)lisCortes->Items[nc];
		if(!dataConfiab->ParamEqpto(p1, pcorte1->eqpto)) continue;
		dcomp = CompEqpto(pcorte1->eqpto);
		if(dcomp > 0. ) {p1.tx_falha *= dcomp;
							  p1.tx_interr*= dcomp;
							 }
      //if(pcorte1->eqpto->Codigo.AnsiCompare("(p3-1)(p2-1)") == 0)
      //   int kkk = 0;
		if(fp) fprintf(fp, "EQPTO 1:\t%s\t%6.5f\t%6.5f\t%6.5f\t%6.5f\t%6.5f\n", pcorte1->eqpto->Codigo.c_str(), p1.tx_falha, p1.tx_interr, p1.treparo, p1.tman, p1.fat_isola);
		if((pcorte1->lisFontes->Count > 0)||(pcorte1->lisFontesP->Count > 0))
			 {CalcInd1(p1, pcorte1->lisFontes, pcorte1->lisFontesP, pcorte1->res_conf);//AlteraHk
			  //if(fp) fprintf(fp, "Corte 1\n");
			  //if(fp) fprintf(fp, "FontesD\n");
			  for(int npp = 0; npp < pcorte1->lisFontes->Count; npp++)
				  {
				  arvs = (VTArvores*)pcorte1->lisFontes->Items[npp];
				  if(fp) fprintf(fp, "%i\t %s\t", npp, arvs->EqRaiz()->Codigo.c_str());
				  if(fp) fprintf(fp, "%6.5f\t %6.5f\t %6.5f\t %6.5f\n", arvs->res_conf.tx_ff, arvs->res_conf.tx_fm, arvs->res_conf.r_ff, arvs->res_conf.r_fm);
				  }
			  //if(fp) fprintf(fp, "FontesP\n");
			  for(int npp = 0; npp < pcorte1->lisFontesP->Count; npp++)
				  {
				  arvs = (VTArvores*)pcorte1->lisFontesP->Items[npp];
				  if(fp) fprintf(fp, "%i\t %s\t", npp, arvs->EqRaiz()->Codigo.c_str());
				  if(fp) fprintf(fp, "%6.5f\t %6.5f\t %6.5f\t %6.5f\n", arvs->res_conf.tx_ff, arvs->res_conf.tx_fm, arvs->res_conf.r_ff * 8760, arvs->res_conf.r_fm * 8760);
				  }
			 }

		for(int n2 = 0; n2 < pcorte1->lisCortes->Count; n2++)
			{
			pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[n2];
			if(!dataConfiab->ParamEqpto(p2, pcorte2->eqpto)) continue;
			dcomp = CompEqpto(pcorte2->eqpto);
			if(dcomp > 0. ) {p2.tx_falha *= dcomp;
								  p2.tx_interr*= dcomp;
								 }
			if(fp) fprintf(fp, "EQPTO 2:\t%s\t%6.5f\t%6.5f\t%6.5f\t%6.5f\t%6.5f\n", pcorte2->eqpto->Codigo.c_str(), p2.tx_falha, p2.tx_interr, p2.treparo, p2.tman, p2.fat_isola);
			if(pcorte2->lisFontes->Count > 0)
			  {CalcInd2(p1, p2, pcorte2->lisFontes, pcorte2->res_conf);
				if(fp) fprintf(fp, "Corte 2\n");
				if(fp) fprintf(fp, "FontesD\n");
				for(int npp = 0; npp < pcorte2->lisFontes->Count; npp++)
					{
					arvs = (VTArvores*)pcorte2->lisFontes->Items[npp];
					if(fp) fprintf(fp, "%i\t %s\t", npp, arvs->EqRaiz()->Codigo.c_str());
					if(fp) fprintf(fp, "%6.5f\t %6.5f\t %6.5f\t %6.5f\t", arvs->res_conf.tx_ff, arvs->res_conf.tx_fm, arvs->res_conf.r_ff * 8760, arvs->res_conf.r_fm * 8760);
					}
			  }
			for(int n3 = 0; n3 < pcorte2->lisCortes->Count; n3++)
				{
				pcorte3 = (TCorteConf*)pcorte2->lisCortes->Items[n3];
				if(!dataConfiab->ParamEqpto(p3, pcorte3->eqpto)) continue;
				dcomp = CompEqpto(pcorte3->eqpto);
				if(dcomp > 0. ) {p3.tx_falha *= dcomp;
									  p3.tx_interr*= dcomp;
									 }
            //if(pcorte3->eqpto->Codigo.AnsiCompare("(p3-1)(p2-1)") == 0)
            //   int kkk = 0;
				if(fp) fprintf(fp, "EQPTO 3:\t%s\t%6.5f\t%6.5f\t%6.5f\t%6.5f\t%6.5f\t", pcorte3->eqpto->Codigo.c_str(), p3.tx_falha, p3.tx_interr, p3.treparo, p3.tman, p3.fat_isola);
				if(pcorte3->lisFontes->Count > 0)
				  {CalcInd3(p1, p2, p3, pcorte3->lisFontes, pcorte3->res_conf);
					//if(fp) fprintf(fp, "Corte 3\n");
					//if(fp) fprintf(fp, "FontesD\n");
					for(int npp = 0; npp < pcorte3->lisFontes->Count; npp++)
						{
						arvs = (VTArvores*)pcorte3->lisFontes->Items[npp];
						//if(fp) fprintf(fp, "%i\t %s\t", npp, arvs->EqRaiz()->Codigo);
						if(fp) fprintf(fp, "%10.8f\t %10.8f\t %6.5f\t %6.5f\n", pcorte3->res_conf.tx_ff, pcorte3->res_conf.tx_fm, pcorte3->res_conf.r_ff * 8760, pcorte3->res_conf.r_fm * 8760);
						}
				  }
				}
			}
		}
   if(fp) fclose(fp);
	for(int n = 0; n < lisArvs->Count; n++)
		{
		arvs = (VTArvores*)lisArvs->Items[n];
		if(arvs->arvs_ori != NULL) continue;
		lisPath = arvs->LisPath(false, NULL);   //PENDENCIA CAMINHOS NA
		arvs->res_conf.fic = 1;
		arvs->res_conf.dic = 8760;
		if(lisPath->Count > 0)
		  {
		  arvs->res_conf.fic = arvs->res_conf.tx_ff  + arvs->res_conf.tx_fm;
		  arvs->res_conf.dic = (arvs->res_conf.r_ff  + arvs->res_conf.r_fm) * 8760;
		  arvs->res_conf.r_ff /= Max(1., arvs->res_conf.tx_ff);
		  arvs->res_conf.r_fm /= Max(1., arvs->res_conf.tx_fm);
		  }
		arvs->res_conf.r_ff /= Max(1., arvs->res_conf.tx_ff);
		arvs->res_conf.r_fm /= Max(1., arvs->res_conf.tx_fm);
		}
	if(prog) prog->Stop();
	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::CalcInd1(PAR_CONF p1, TList* lisFontes, TList* lisFontesP, RES_MC &res)
	{
	VTArvores* arvs;
	double p1tr, p1tm;
	double p1tf, p1ti;
	double tx_ff, tx_fm, r_ff, r_fm, r_ffIsola;
	double fat_isola;

	p1tr = p1.treparo / 8760;
	p1tm = p1.tman / 8760;
	p1tf = p1.tx_falha;
	p1ti = p1.tx_interr;
	fat_isola = p1.fat_isola;

	//if(IsDoubleZero(p1tr)) return(false);
	//if(IsDoubleZero(p1tf)) return(false);



	tx_ff  = p1tf;
	tx_fm  = p1ti;
	r_ff   = p1tr;
	r_fm   = p1tm;

	res.tx_ff = tx_ff;
	res.tx_fm = tx_fm;
	res.r_ff  = r_ff * 8760;
	res.r_fm  = r_fm * 8760;

	for(int n = 0; n <lisFontes->Count; n++)
		{
		arvs = (VTArvores*)lisFontes->Items[n];
		arvs->res_conf.tx_ff  +=  tx_ff;
		arvs->res_conf.tx_fm  +=  tx_fm;
		arvs->res_conf.r_ff   +=  r_ff * tx_ff;//+=  r_ff;
		arvs->res_conf.r_fm   +=  r_fm * tx_fm;//+=  r_fm;
		}

	if(lisFontesP != NULL)
	  {
	  r_ffIsola = r_ff * fat_isola;
	  for(int n = 0; n <lisFontesP->Count; n++)
		  {
		  arvs = (VTArvores*)lisFontesP->Items[n];
		  arvs->res_conf.tx_ff  +=  tx_ff;
		  arvs->res_conf.tx_fm  +=  tx_fm;
		  arvs->res_conf.r_ff   +=  r_ffIsola * tx_ff;//+=r_ffIsola
		  arvs->res_conf.r_fm   +=  r_fm      * tx_fm;//+=r_fm
		  }
	  }

	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::CalcInd2(PAR_CONF p1, PAR_CONF p2, TList* lisFontes, RES_MC &res)
	{
	VTArvores* arvs;
	double p1tr, p1tm, p2tr, p2tm;
	double p1tf, p1ti, p2tf, p2ti;
	double lambda_f1, lambda_f2, lambda_m1, lambda_m2;
	double ni12, ni21;
	double tx_ff, tx_fm, r_ff, r_fm;

	p1tr = p1.treparo / 8760; p1tm = p1.tman / 8760;
	p2tr = p2.treparo / 8760; p2tm = p2.tman / 8760;
	p1tf = p1.tx_falha;
	p1ti = p1.tx_interr;
	p2tf = p2.tx_falha;
	p2ti = p2.tx_interr;

	if((IsDoubleZero(p1tr))||(IsDoubleZero(p2tr))) return(false);
	if((IsDoubleZero(p1tf))||(IsDoubleZero(p2tf))) return(false);

	lambda_f1   = p1tf * p1tr;
	lambda_f2   = p2tf * p2tr;
	lambda_m1   = p1ti * p1tm;
	lambda_m2   = p2ti * p2tm;

	//2 falhas sobrepostas
	res.tx_ff= tx_ff = p2tf * lambda_f1 + p1tf * lambda_f2;
	r_ff  = (p1tr * p2tr) / (p1tr + p2tr);
	res.r_ff = r_ff * 8760;
	//1 falha sobreposta a uma manutenção
	ni12  = p1tm /(p1tm + p2tr);
	ni21  = p2tm /(p2tm + p1tr);
	res.tx_fm = tx_fm = p2tf * lambda_m1 + p1tf * lambda_m2;
	r_fm  = (IsDoubleZero(tx_fm, 1E-10)) ? 0.0 : (lambda_m1 * lambda_f2 * ni12 + lambda_m2 * lambda_f1 * ni21) / tx_fm;
	res.r_fm = r_fm * 8760;
	for(int n = 0; n <lisFontes->Count; n++)
		{
		arvs = (VTArvores*)lisFontes->Items[n];
		arvs->res_conf.tx_ff  +=  tx_ff;
		arvs->res_conf.tx_fm  +=  tx_fm;
		arvs->res_conf.r_ff   +=  r_ff * tx_ff;//+=  r_ff;
		arvs->res_conf.r_fm   +=  r_fm * tx_fm;//+=  r_fm;
		}

	return(true);
	}
//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::CalcInd3(PAR_CONF p1, PAR_CONF p2, PAR_CONF p3, TList* lisFontes, RES_MC &res)
	{
	VTArvores* arvs;
	double p1tr, p1tm, p2tr, p2tm, p3tr, p3tm;
	double p1tf, p1ti, p2tf, p2ti, p3tf, p3ti;
	double lambda_f1, lambda_f2, lambda_f3, lambda_m1, lambda_m2, lambda_m3;
	double tx_fff, r_fff, tx_ffm, r_ffm;
	double ni12, ni13, ni21, ni23, ni31, ni32;
	double a123, a312, a231, w123, w312, w231;



	p1tr = p1.treparo / 8760; p1tm = p1.tman / 8760;
	p2tr = p2.treparo / 8760; p2tm = p2.tman / 8760;
	p3tr = p3.treparo / 8760; p3tm = p3.tman / 8760;

	/*p1tr = 20. / 8760.; p1tm = 	10. / 8760.;
	p2tr = 20. / 8760.; p2tm =   10. / 8760.;
	p3tr = 20. / 8760.; p3tm =   10. / 8760.;
	*/

	if((IsDoubleZero(p1tr))||(IsDoubleZero(p2tr))||(IsDoubleZero(p3tr))) return(false);
	if((IsDoubleZero(p1tf))||(IsDoubleZero(p2tf))||(IsDoubleZero(p3tf))) return(false);

	p1tf = p1.tx_falha;
	p1ti = p1.tx_interr;
	p2tf = p2.tx_falha;
	p2ti = p2.tx_interr;
	p3tf = p3.tx_falha;
	p3ti = p3.tx_interr;


	/*p1tf = 0.01;
	p1ti = 0.10;
	p2tf = 0.01;
	p2ti = 0.10;
	p3tf = 0.01;
	p3ti = 0.10;
	*/

	lambda_f1   = p1tf * p1tr;
	lambda_f2   = p2tf * p2tr;
	lambda_f3   = p3tf * p3tr;
	lambda_m1   = p1ti * p1tm;
	lambda_m2   = p2ti * p2tm;
	lambda_m3   = p3ti * p3tm;
	if((IsDoubleZero(lambda_f1, 1E-10))&&(IsDoubleZero(lambda_m1, 1E-10))) return(false);
	if((IsDoubleZero(lambda_f2, 1E-10))&&(IsDoubleZero(lambda_m2, 1E-10))) return(false);
	if((IsDoubleZero(lambda_f3, 1E-10))&&(IsDoubleZero(lambda_m3, 1E-10))) return(false);

	//3 falhas sobrepostas
	tx_fff = lambda_f1 * lambda_f2 * p3tf;
	tx_fff+= lambda_f2 * lambda_f3 * p1tf;
	tx_fff+= lambda_f3 * lambda_f1 * p2tf;
	r_fff  = (p1tr * p2tr * p3tr);
	r_fff /= (p1tr * p2tr + p2tr * p3tr + p3tr * p1tr);
	//2 falhas sobrepostas a uma manutenção
	ni12  = p1tm /(p1tm + p2tr);
	ni13  = p1tm /(p1tm + p3tr);
	ni31  = p3tm /(p3tm + p1tr);
	ni32  = p3tm /(p3tm + p2tr);
	ni23  = p2tm /(p2tm + p3tr);
	ni21  = p2tm /(p2tm + p1tr);

	a123 = lambda_m1 * (lambda_f2 * p3tf * ni12 + lambda_f3 * p2tf * ni13);
	a312 = lambda_m3 * (lambda_f1 * p2tf * ni31 + lambda_f2 * p1tf * ni32);
	a231 = lambda_m2 * (lambda_f3 * p1tf * ni23 + lambda_f1 * p3tf * ni21);

	w123 = (p1tm * p2tr * p3tr) / (p1tm * p2tr + p2tr * p3tr + p3tr * p1tm);
	w312 = (p3tm * p1tr * p2tr) / (p3tm * p1tr + p1tr * p2tr + p2tr * p3tm);
	w231 = (p2tm * p3tr * p1tr) / (p2tm * p3tr + p3tr * p1tr + p1tr * p2tm);

	tx_ffm = a123 + a231 + a312;
	r_ffm  = IsDoubleZero(tx_ffm, 1E-10) ? 0.0 : (a123 * w123 + a231 * w231 + a312 * w312) / tx_ffm;

	res.tx_ff = tx_fff;
	res.tx_fm = tx_ffm;
	res.r_ff  = r_fff * 8760;
	res.r_fm  = r_ffm * 8760;


	for(int n = 0; n <lisFontes->Count; n++)
		{
		arvs = (VTArvores*)lisFontes->Items[n];
		arvs->res_conf.tx_ff  +=  tx_fff;
		arvs->res_conf.tx_fm  +=  tx_ffm;
		arvs->res_conf.r_ff   +=  r_fff * tx_fff;//+=  r_fff;
		arvs->res_conf.r_fm   +=  r_ffm * tx_ffm;//+=  r_ffm;
		}


	return(true);
	}
/*//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::Executa(VTArvores* arvs)
	{
	VTDadoConfiab* dataConfiab = (VTDadoConfiab*)apl->GetObject(__classid(VTDadoConfiab));
	double dic, fic;
	double lambda_f1, lambda_f2, lambda_f3, lambda_m1, lambda_m2, lambda_m3;
	double tx_ff, r_ff, tx_fm, r_fm, tx_fff, r_fff, tx_ffm, r_ffm;
	double ni12, ni13, ni31, ni32, ni23, ni21;
	double a123, a312, a231, w123, w312, w231;
	TList*   lisCorte;
	TCorteConf*  pcorte;
	PAR_CONF p1, p2, p3;
	double  p1tr, p1tm, p2tr, p2tm, p3tr, p3tm;
	double  p1tf, p1ti, p2tf, p2ti, p3tf, p3ti;
	double  dc1, dc2, dc3, dcomp;
	VTEqpto *eq1, *eq2, *eq3;


	//Inicia parametros confiabilidade para a carga estudada
	arvs->res_conf.dic = 0.;
	arvs->res_conf.fic = 0.;
	arvs->res_conf.tx_ff = 0.;
	arvs->res_conf.tx_fm = 0.;
	arvs->res_conf.r_ff  = 0.;
	arvs->res_conf.r_fm  = 0.;

	//Cortes de primeira ordem
	lisCorte = arvs->LisCortes(1);
	for(int n = 0; n < lisCorte->Count; n++)
		  {
		  pcorte = (TCorteConf*)lisCorte->Items[n];
		  //hkConf p1 = confiab->ParEqpto((VTEqpto*)pcorte->lisEqpto->Items[0]);
		  eq1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
		  if(!dataConfiab->ParamEqpto(p1, eq1)) continue;

		  p1tr = p1.treparo / 8760;
		  p1tm = p1.tman / 8760;
		  p1tf = p1.tx_falha;
		  p1ti = p1.tx_interr;
		  //if(eq1->Tipo() == eqptoTRECHO)
		  if((dcomp = CompEqpto(eq1)) > 0.)
			 {p1tf *= dcomp;
			  p1ti *= dcomp;
			 }
		  pcorte->res_conf.dic   = (p1tf * p1tr + p1ti * p1tm) * 8760;
		  pcorte->res_conf.fic   = p1tf + p1ti;
		  pcorte->res_conf.tx_ff = p1tf;
		  pcorte->res_conf.tx_fm = p1ti;
		  pcorte->res_conf.r_ff  = p1tr;
		  pcorte->res_conf.r_fm  = p1tm;

		  arvs->res_conf.tx_ff  += pcorte->res_conf.tx_ff;
		  arvs->res_conf.tx_fm  += pcorte->res_conf.tx_fm;
		  arvs->res_conf.r_ff   += pcorte->res_conf.tx_ff * pcorte->res_conf.r_ff;
		  arvs->res_conf.r_fm   += pcorte->res_conf.tx_fm * pcorte->res_conf.r_fm;
		  }
	//Cortes de segunda ordem
	lisCorte = arvs->LisCortes(2);
	for(int n = 0; n < lisCorte->Count; n++)
		  {
		  pcorte = (TCorteConf*)lisCorte->Items[n];
		  eq1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
		  eq2 = (VTEqpto*)pcorte->lisEqpto->Items[1];
		  if(!dataConfiab->ParamEqpto(p1, eq1)) continue;
		  if(!dataConfiab->ParamEqpto(p2, eq2)) continue;

		  p1tr = p1.treparo / 8760; p1tm = p1.tman / 8760;
		  p2tr = p2.treparo / 8760; p2tm = p2.tman / 8760;
		  p1tf = p1.tx_falha;
		  p1ti = p1.tx_interr;
		  p2tf = p2.tx_falha;
		  p2ti = p2.tx_interr;
		  if((dcomp = CompEqpto(eq1)) > 0.)
			 {p1tf *= dcomp;
			  p1ti *= dcomp;
		  	 }
		  if((dcomp = CompEqpto(eq2)) > 0.)
		  	 {p2tf *= dcomp;
		  	  p2ti *= dcomp;
			 }


		  lambda_f1   = p1tf * p1tr;
		  lambda_f2   = p2tf * p2tr;
		  lambda_m1   = p1ti * p1tm;
		  lambda_m2   = p2ti * p2tm;
		  //2 falhas sobrepostas
		  tx_ff = p2tf * lambda_f1 + p1tf * lambda_f2;
		  r_ff  = (p1tr * p2tr) / (p1tr + p2tr);
		  //1 falha sobreposta a uma manutenção
		  ni12  = p1tm /(p1tm + p2tr);
		  ni21  = p2tm /(p2tm + p1tr);
		  tx_fm = p2tf * lambda_m1 + p1tf * lambda_m2;
		  r_fm  = (lambda_m1 * lambda_f2 * ni12 + lambda_m2 * lambda_f1 * ni21) / tx_fm;

		  pcorte->res_conf.dic   = (tx_ff * r_ff + tx_fm * r_fm) * 8760;
		  pcorte->res_conf.fic   = (tx_ff + tx_fm);
		  pcorte->res_conf.tx_ff = tx_ff;
		  pcorte->res_conf.tx_fm = tx_fm;
		  pcorte->res_conf.r_ff  = r_ff;
		  pcorte->res_conf.r_fm  = r_fm;

		  arvs->res_conf.tx_ff  += pcorte->res_conf.tx_ff;
		  arvs->res_conf.tx_fm  += pcorte->res_conf.tx_fm;
		  arvs->res_conf.r_ff   += pcorte->res_conf.tx_ff * pcorte->res_conf.r_ff;
		  arvs->res_conf.r_fm   += pcorte->res_conf.tx_fm* pcorte->res_conf.r_fm;
		  }

	//Cortes de terceira ordem
	lisCorte = arvs->LisCortes(3);
	for(int n = 0; n < lisCorte->Count; n++)
		  {
		  pcorte = (TCorteConf*)lisCorte->Items[n];
		  eq1 = (VTEqpto*)pcorte->lisEqpto->Items[0];
		  eq2 = (VTEqpto*)pcorte->lisEqpto->Items[1];
		  eq3 = (VTEqpto*)pcorte->lisEqpto->Items[2];
		  if(!dataConfiab->ParamEqpto(p1, (VTEqpto*)pcorte->lisEqpto->Items[0])) continue;
		  if(!dataConfiab->ParamEqpto(p2, (VTEqpto*)pcorte->lisEqpto->Items[1])) continue;
		  if(!dataConfiab->ParamEqpto(p3, (VTEqpto*)pcorte->lisEqpto->Items[2])) continue;


		  p1tr = p1.treparo / 8760; p1tm = p1.tman / 8760;
		  p2tr = p2.treparo / 8760; p2tm = p2.tman / 8760;
		  p3tr = p3.treparo / 8760; p3tm = p3.tman / 8760;
		  p1tf = p1.tx_falha;
		  p1ti = p1.tx_interr;
		  p2tf = p2.tx_falha;
		  p2ti = p2.tx_interr;
		  p3tf = p3.tx_falha;
		  p3ti = p3.tx_interr;



		  if((dcomp = CompEqpto(eq1)) > 0.)
			 {p1tf *= dcomp;
			  p1ti *= dcomp;
			 }
		  if((dcomp = CompEqpto(eq2)) > 0.)
			 {p2tf *= dcomp;
			  p2ti *= dcomp;
			 }
		  if((dcomp = CompEqpto(eq3)) > 0.)
			 {p3tf *= dcomp;
			  p3ti *= dcomp;
			 }

		  lambda_f1   = p1tf * p1tr;
		  lambda_f2   = p2tf * p2tr;
		  lambda_f3   = p3tf * p3tr;
		  lambda_m1   = p1ti * p1tm;
		  lambda_m2   = p2ti * p2tm;
		  lambda_m3   = p3ti * p3tm;

		  //3 falhas sobrepostas
		  tx_fff = lambda_f1 * lambda_f2 * p3tf;
		  tx_fff+= lambda_f2 * lambda_f3 * p1tf;
		  tx_fff+= lambda_f3 * lambda_f1 * p2tf;
		  r_fff  = (p1tr * p2tr * p3tr);
		  r_fff /= (p1tr * p2tr + p2tr * p3tr + p3tr * p1tr);
		  //2 falhas sobrepostas a uma manutenção
		  ni12  = p1tm /(p1tm + p2tr);
		  ni13  = p1tm /(p1tm + p3tr);
		  ni31  = p3tm /(p3tm + p1tr);
		  ni32  = p3tm /(p3tm + p2tr);
		  ni23  = p2tm /(p2tm + p3tr);
		  ni21  = p2tm /(p2tm + p1tr);

		  a123 = lambda_m1 * (lambda_f2 * p3tf * ni12 + lambda_m3 * p2tf * ni13);
		  a312 = lambda_m3 * (lambda_f1 * p2tf * ni31 + lambda_m2 * p1tf * ni32);
		  a231 = lambda_m2 * (lambda_f3 * p1tf * ni23 + lambda_m1 * p3tf * ni21);

		  w123 = (p1tm * p2tr * p3tr) / (p1tm * p2tr + p2tr * p3tr + p3tr * p1tm);
		  w312 = (p3tm * p1tr * p2tr) / (p3tm * p1tr + p1tr * p2tr + p2tr * p3tm);
		  w231 = (p2tm * p3tr * p1tr) / (p2tm * p3tr + p3tr * p1tr + p1tr * p2tm);

		  tx_ffm = a123 + a231 + a312;
		  r_ffm  = (a123 * w123 + a231 * w231 + a312 * w312) / tx_ffm;

		  pcorte->res_conf.dic   = (tx_fff * r_fff + tx_ffm * r_ffm) * 8760;
		  pcorte->res_conf.fic   = (tx_fff + tx_ffm);
		  pcorte->res_conf.tx_ff = tx_fff;
		  pcorte->res_conf.tx_fm = tx_ffm;
		  pcorte->res_conf.r_ff  = r_fff;
		  pcorte->res_conf.r_fm  = r_ffm;

		  arvs->res_conf.tx_ff  += pcorte->res_conf.tx_ff;
		  arvs->res_conf.tx_fm  += pcorte->res_conf.tx_fm;
		  arvs->res_conf.r_ff   += pcorte->res_conf.tx_ff * pcorte->res_conf.r_ff;
		arvs->res_conf.r_fm   += pcorte->res_conf.tx_fm* pcorte->res_conf.r_fm;
		}
	TList* lisPath = arvs->LisPath(false, NULL);   //PENDENCIA CAMINHOS NA
	if(lisPath->Count == 0)
	 {
	 arvs->res_conf.fic = 1;
	 arvs->res_conf.dic = 8760;
	 arvs->res_conf.r_ff /= Max(1., arvs->res_conf.tx_ff);
	 arvs->res_conf.r_fm /= Max(1., arvs->res_conf.tx_fm);
	 }
	else
	 {
	 arvs->res_conf.fic = arvs->res_conf.tx_ff  + arvs->res_conf.tx_fm;
	 arvs->res_conf.dic = (arvs->res_conf.r_ff  + arvs->res_conf.r_fm) * 8760;
	 arvs->res_conf.r_ff /= Max(1., arvs->res_conf.tx_ff);
	 arvs->res_conf.r_fm /= Max(1., arvs->res_conf.tx_fm);
	 }
	return(true);
	}
*/
//---------------------------------------------------------------------------
bool __fastcall TCalcIndices::AgregaRes(RES_CONF &res_conf, TList* lisArv)
	{
	double    ptot; // daux, faux,
	int       naux;
	VTArvores* arvs;
	VTEqpto   *eqpto, *eqpto_aux;

	if(lisArv->Count == 0) return(false);
	res_conf.dec = res_conf.fec = res_conf.end = 0.;
	res_conf.ncons = 0;
	ptot   = 0;  //faux = daux =
	for(int n = 0; n < lisArv->Count; n++)
		{
		arvs = (VTArvores*)lisArv->Items[n];
		//if(arvs->EqRaiz()->Tipo() != eqptoCARGA) return(false);

		//daux = res_conf.dec * res_conf.ncons;
		//faux = res_conf.fec * res_conf.ncons;

		eqpto = arvs->EqRaiz();//(arvs->arvs_ori == NULL) ? arvs->EqRaiz() : arvs->arvs_ori->EqRaiz();
		//if(arvs->EqRaiz()->Tipo() == eqptoCARGA)
		if(eqpto->Tipo() == eqptoCARGA)
		  {
		  naux = NCons(arvs->EqRaiz());
		  if(naux <= 0) naux = 1;

		  //if((naux = NCons(arvs->EqRaiz())) <= 0) continue;//return(false);
		  }
		else
		  {
		  //Caso de redes reduzidas
		  naux = 1;
		  }

		arvs->res_conf.ncons = naux;

		res_conf.dec   += (arvs->res_conf.dic * naux);
		res_conf.fec   += (arvs->res_conf.fic * naux);
		//Calcula a end baseado nas demandas da curva de carga - talvez rodar fluxo e pegar
		//resultados de resflow?

		res_conf.end += (arvs->res_conf.pmed = PotMedia(arvs->EqRaiz())) * arvs->res_conf.dic;
		ptot += arvs->res_conf.pmed;
		res_conf.ncons += naux;
		}

   if(res_conf.ncons == 0) res_conf.ncons = 1;

	res_conf.clih = res_conf.dec;
	res_conf.cli  = res_conf.fec;

	res_conf.dec  /= res_conf.ncons;
	res_conf.fec  /= res_conf.ncons;

	res_conf.pmed_mw = ptot;//:(res_conf.dec > 0) ? res_conf.end / res_conf.dec : 0.;
	//res_conf.end    *= 8760;

	return(true);
	}
//---------------------------------------------------------------------------
double __fastcall TCalcIndices::CompEqpto(VTEqpto* eqpto)
	{
	TMetaClass *obj_class;
	AnsiString obj_name;
	double dcomp = 0.0;

	if(eqpto->Tipo() == eqptoTRECHO)
	  {
	  dcomp = ((VTTrecho*)eqpto)->Comprimento_km;
	  }
	else
	  {
	  obj_class = eqpto->ClassType();
	  obj_name  = obj_class->ClassName();
	  if(obj_name.AnsiCompareIC("TLigaSerie") == 0)
		 dcomp = ((TLigaSerie*)eqpto)->Comprimento_km;
	  }
	return(dcomp);
	}
//---------------------------------------------------------------------------
int __fastcall TCalcIndices::NCons(VTEqpto* eqpto)
	{
	int ncons;
	VTCarga *pcarga;

	if(eqpto->Tipo() != eqptoCARGA) return(-1);
	pcarga = (VTCarga*)eqpto;
	ncons = (pcarga->Tipo() == consEQUIVALENTE) ? pcarga->NumConsTotal : 1;


	return(ncons);
   }
//---------------------------------------------------------------------------
double __fastcall TCalcIndices::PotMedia(TList* lisEqpto)
	{
	VTEqpto* eqpto;
	double   pmed;

	pmed = 0.;
	for(int n = 0; n < lisEqpto->Count; n++)
		{
		eqpto = (VTEqpto*)lisEqpto->Items[n];
		pmed += PotMedia(eqpto);
		}
	return(pmed);
	}

//---------------------------------------------------------------------------
double __fastcall TCalcIndices::PotMedia(VTEqpto* eqpto)
	{
	double pmed;

	if(eqpto->Tipo() == eqptoCARGA)
		 pmed  = PotMediaCarga(eqpto);
	else
		 pmed = PotMediaReducao(eqpto);

	return(pmed);
	}
	
//---------------------------------------------------------------------------
double __fastcall TCalcIndices::PotMediaCarga(VTEqpto* eqpto)
	{
	VTDemanda* demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   double pmed;
   VTPatamares* patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int          num_pat   = patamares->NumPatamar();
	double       daux;

	//pmed em mw
	if(eqpto->Tipo() != eqptoCARGA) return(0.);
	//pmed = pdemanda->Edia_mwh[eqpto] / 24.;
	//return(pmed);
	daux = pmed = 0;
	for (int np = 0; np < num_pat; np++)
		 {//determina demanda nominal da Carga no patamar
			pmed += demanda->P_mw[eqpto][np] * patamares->Patamar[np]->Duracao;
			daux += patamares->Patamar[np]->Duracao;
		 }

	pmed = (daux > 0) ? pmed/ daux : 0;
	return(pmed);
	}
	
//---------------------------------------------------------------------------
double __fastcall TCalcIndices::PotMediaReducao(VTEqpto* eqpto)
	{
	VTReducao    *reducao;
	VTMRede      *mrede;
	TList        *lisCarga;
	double       pmed;

	reducao = BuscaReducao(eqpto);
	if(reducao == NULL) return(0.);
	lisCarga = new TList();
	reducao->MRede->LisEqbar(lisCarga, eqptoCARGA);
	pmed = PotMedia(lisCarga);
	delete lisCarga;

	return(pmed);
	}
//---------------------------------------------------------------------------
VTReducao* __fastcall TCalcIndices::BuscaReducao(VTEqpto* eqpto)
	{
	VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTReducao *redu;
	TList     *lisRedu, *lisBarra;
	VTEqpto   *eqpto_aux;

	lisRedu = new TList();
	redes->LisReducao(lisRedu);
	redu = NULL;
	for(int nr = 0; nr < lisRedu->Count; nr++)
		{
		redu = (VTReducao*)lisRedu->Items[nr];
		if(redu->ExisteGeracao()) continue;
		lisBarra = redu->LisBarra();
		if(lisBarra->Count == 0) continue;
		eqpto_aux = (VTEqpto*)lisBarra->Items[0];
		if(eqpto == eqpto_aux) break;
		redu = NULL;
		}

	delete lisRedu;
	return(redu);
	}
//---------------------------------------------------------------------------

