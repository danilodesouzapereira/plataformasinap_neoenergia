//---------------------------------------------------------------------------
#ifndef TSegmentoH
#define TSegmentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSegmento.h"

//---------------------------------------------------------------------------
class VTArea;
class VTRede;

//---------------------------------------------------------------------------
class TSegmento : public VTSegmento
   {
   public:  //funções públicas
                     __fastcall TSegmento(void);
                     __fastcall ~TSegmento(void);
      VTCrescimento* __fastcall ExisteCrescimento(VTClasse *classe);
      void           __fastcall IniciaCrescimentoNulo(void);
      bool           __fastcall InsereCrescimento(VTCrescimento *crescimento);
      TList*         __fastcall LisCrescimento(void);
      void           __fastcall RedefinePeriodo(int ano_ini, int ano_fim);

   protected:  //métodos acessados via property
      VTArea*    __fastcall PM_GetArea(void);
      bool       __fastcall PM_GetAlterada(void);
		AnsiString __fastcall PM_GetCodigo(void);
		bool       __fastcall PM_GetEnabled(void);
      int        __fastcall PM_GetId(void);
		bool       __fastcall PM_GetNova(void);
		VTRegiao*  __fastcall PM_GetRegiao(void);
		int        __fastcall PM_GetTipo(void);
      void       __fastcall PM_SetAlterada(bool alterada);
      void       __fastcall PM_SetArea(VTArea *area);
		void       __fastcall PM_SetCodigo(AnsiString codigo);
		void       __fastcall PM_SetEnabled(bool enabled);
      void       __fastcall PM_SetId(int Segmento_id);
		void       __fastcall PM_SetNova(bool nova);
		void       __fastcall PM_SetRegiao(VTRegiao *regiao);
		void       __fastcall PM_SetStatus(unsigned bit, bool enabled);
		void       __fastcall PM_SetTipo(int tipo);

   private:  //objetos externos
      VTRede *rede;

   private:  //dados acessados via property
		struct   {
					int        id;
					int        status;
					int        tipo;
               AnsiString codigo;
               VTArea     *area;
					VTRede     *rede;
					VTRegiao   *regiao;
               } PD;

   private:  //dados locais
      TList *lisCRES;
   };

//---------------------------------------------------------------------------
#endif
//eof
