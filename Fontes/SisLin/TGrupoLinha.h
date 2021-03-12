
//---------------------------------------------------------------------------
#ifndef TGrupoLinhaH
#define TGrupoLinhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TGrupoLinha : public TObject
   {
   public:  // Métodos
                         __fastcall  TGrupoLinha(int num_elem_FD,
                                                 int linha,
                                                 int vet_ind_linha[]);
                         __fastcall ~TGrupoLinha(void);
                    void __fastcall  AddLinha(int linha,
                                              int vet_ind_linha[]);
                    int  __fastcall  Linha(int indice);
                    int  __fastcall  NumElemFD(void);
                    int  __fastcall  NumLinhas(void);
                    void __fastcall  RemoveLinha(int indice,
                                                 int vet_ind_linha[]);

   private:  // Métodos
      void * __fastcall VoidPointer(int num1);

   private:  // Dados
      int   num_elem_FD;  // Número de elementos FD do grupo
      TList *lisLINHA;    // Lista de linhas do grupo
   };

#endif
//---------------------------------------------------------------------------
//eof
