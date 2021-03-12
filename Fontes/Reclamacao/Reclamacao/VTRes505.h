//---------------------------------------------------------------------------
#ifndef VTRes505H
#define VTRes505H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
enum enumRA505_VNOM {ra505_GE230KV = 1,
                     ra505_LT230KV_GE69KV,
                     ra505_LT69KV_GT1KV,
                     ra505_LE1KV_TRI_220_127,
                     ra505_LE1KV_TRI_380_220,
                     ra505_LE1KV_MON_254_127,
                     ra505_LE1KV_MON_440_220,
                     //não padronizadas
                     ra505_LE1KV_TRI_208_120,
                     ra505_LE1KV_MON_230_115,
                     ra505_LE1KV_MON_240_120,
                     //padrão Eletropaulo
                     ra505_LE1KV_MON_220_110,
                     //padrão AES Sul
                     ra505_LE1KV_MON_110_110,
							ra505_LE1KV_MON_220_220,
							//default //DVK 2015.12.07
							ra505_DEFAULT
                     };

//---------------------------------------------------------------------------
enum enum_leituras {eADEQUADA = 0, ePRECARIA, eCRITICA};
enum enum_tensoes  {eFN = 0, eFF};

//---------------------------------------------------------------------------
class VTRes505 : public TObject
   {
   public:  //property
		__property AnsiString Descricao  	= {read=descricao};
		__property int        Tensao     	= {read=tensoes.tensao,  write=tensoes.tensao};
		__property double		 VcontratadaMT = {read=vContratadaMT_kv,write=vContratadaMT_kv};
		__property double     Vnom_ff_kv 	= {read=tensoes.vff,     write=tensoes.vff   };
		__property double     Vnom_fn_kv 	= {read=tensoes.vfn,     write=tensoes.vfn   };

   public:
						 __fastcall	VTRes505(void)  {};
						 __fastcall	~VTRes505(void) {};
		//virtual void __fastcall  DefineFaixaTensao(int ra505_vnom, double vff_kv, double vfn_kv) = 0;
		virtual void __fastcall	DefineFaixaTensao(int ra505_vnom) = 0;
		virtual void __fastcall	DefineFaixaTensao(double vff_kv, double vfn_kv) = 0;
		virtual int  __fastcall	ClassificaAdequadaPrecariaCritica(double tensao) = 0;

   public:
      struct   {
               double inf, sup;
               }adequado, precario1, precario2, critico;

   protected:
		AnsiString descricao;    //descrição da norma
		double vContratadaMT_kv;
		//double     vnom;
		struct   {
               int    tensao;
               double vfn, vff;
					}tensoes;
   };

//---------------------------------------------------------------------------
VTRes505* __fastcall NewObjRes505(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof

