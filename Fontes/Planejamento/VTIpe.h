//---------------------------------------------------------------------------
#ifndef VTIpeH
#define VTIpeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422298
//https://sinapsisenergia.teamwork.com/#tasks/14422299
//https://sinapsisenergia.teamwork.com/#tasks/14422301
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
enum atributoIPE
				{
					ipePADRAO=1,
					ipeSOLICITACAO=2,
					ipeOBRA,
					ipeREGIAO,
					ipeCROQUI,
					ipePROGRAMA,
					ipeJUSTIFICATIVA,
					ipeMUNICIPIO,
					ipeMUNICIPIO_VPO,
					//atributos novos por causa do item
					ipeITEM_NOME,
					ipeITEM_CONFIGURADO,
					ipeITEM_CONTABILIZAR,
					ipeITEM_REDUCAOPNT,
					ipeITEM_ALTERATAXAFALHA,
					ipeITEM_TAXAFALHAINS,
					ipeITEM_TAXAFALHASUB,
					//atributos novos por causa do item aries
					ipeITEMARIES_QNTINS,
					ipeITEMARIES_QNTRET,
					//novo
					ipeITEM_TIPOACAO,
					ipeITEM_CUSTO,
					//copel
                    ipeOBRA_GRUPO
				};
enum tipoPADRAO  {padraoINS=100, padraoRET=200};

//---------------------------------------------------------------------------
class VTIpeAtributo : public TObject
   {
   public:
	  __property int Tipo = {read=PD.tipo, write=PD.tipo};
	  __property int Id   = {read=PD.id,   write=PD.id};
	  __property int Item_Id   = {read=PD.item_id,   write=PD.item_id};
	  __property AnsiString Descricao  = {read=PD.descricao,   write=PD.descricao};

   public:
               __fastcall  VTIpeAtributo(void) {PD.tipo=-1; PD.id=-1;};
	  virtual  __fastcall ~VTIpeAtributo(void) {};

   protected:  //dados
	  struct{
			int item_id;
            int tipo;
			int id;
			AnsiString descricao;
			}PD;
   };

//---------------------------------------------------------------------------
class VTIpe : public TObject
   {
   public:
                              __fastcall  VTIpe(void) {};
      virtual                 __fastcall ~VTIpe(void) {};
      virtual VTIpeAtributo*  __fastcall  ExisteIpeAtributo(int tipo, int id, int item_id, AnsiString descricao="") = 0;
   };

//---------------------------------------------------------------------------
#endif
//eof



 