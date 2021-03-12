//---------------------------------------------------------------------------
#ifndef TCartografiaH
#define TCartografiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCartografia.h"

//---------------------------------------------------------------------------
class TFormMapa;

//---------------------------------------------------------------------------
class TCartografia : public VTCartografia
{
public:
                __fastcall TCartografia(VTApl *apl, TWinControl *parent,
                                        TAction *ActionMapaDisp);
                __fastcall ~TCartografia(void);
    // jss20191014
    void        __fastcall DatumConvert
                            (double  LatIn, double  LongIn, double  HtIn,
                             double &LatTo, double &LongTo, double &HtTo);
    bool        __fastcall CarregaArea_utm(int map_type, VTArea *area_utm,
                                            VTQuad *quad_pixel, bool &disponivel,
                                            int tipo_coord);
    bool        __fastcall CopiaBitmap(VTArea *area_utm, TCanvas *canvas,
                                        int x1, int y1, int x2, int y2,
                                        double fat_escala, int tipo_coord);
    bool        __fastcall DefineEscala(VTArea *area_utm, VTQuad *quad_pixel);
    void        __fastcall Redimensiona(int width, int height);

protected: // métodos acessados via property
    double      __fastcall PM_GetEscala_pixel_cm(void);
    int         __fastcall PM_GetTipoMapa(void);
    AnsiString  __fastcall PM_GetZonaUtm(void);
    void        __fastcall PM_SetTipoMapa(int map_type);
    void        __fastcall PM_SetZonaUtm(AnsiString zona);

private: // métodos
    bool        __fastcall CopiaBitmapCopel(VTArea *area_utm, TCanvas *canvas,
                                            int x1, int y1, int x2, int y2,
                                            double fat_escala, int tipo_coord);
    double      __fastcall ZoomGoogleHeight(double lat1, double lat2);
    double      __fastcall ZoomGoogleWidth (double lon1, double lon2);

    // objetos externos
    VTApl *apl;

    // dados acessados via property
    struct
    {
        int     map_type;
        double  escala;
    }
    PD;

private: // dados
    TFormMapa   *FormMapa;
    VTScg       *scg;
    double      z0; // relação pixel/grau do zoom 0 do google,
                    // empírico = 0,713

    struct
    {
        double  zoom;
        VTArea  *area_utm;
        VTQuad  *quad_pixel;
    }
    goog;

    struct
    {
        double  lat1, lat2, lat_med;
        double  lon1, lon2, lon_med;
    }
    area_ll;

    struct
    {
        struct
        {
            int width, height;
        }
        saida;
    }
    dim;
};

#endif
//---------------------------------------------------------------------------
//eof
