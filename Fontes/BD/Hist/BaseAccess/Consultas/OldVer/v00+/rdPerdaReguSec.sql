  SELECT
    PerdaReguSec.intRedeId                                                AS RedeId,
    PerdaReguSec.dttData                                                  AS Data,
    PerdaReguSec.fltFatPotencia                                           AS FatPotencia,
    PerdaReguSec.fltFatCarga                                              AS FatCarga,
    PerdaReguSec.fltFatCargaLevePesada                                    AS FatCargaLevePesada,
    PerdaReguSec.fltFatUtilizacao                                         AS FatUtilizacao,
    PerdaReguSec.fltPotNominal                                            AS PotNominal,
    PerdaReguSec.fltCarregamento                                          AS Carregamento,
    PerdaReguSec.fltComprimento                                           AS Comprimento,
    PerdaReguSec.fltTaxaCrescimento                                       AS TaxaCrescimento,
    PerdaReguSec.fltVNom_AT                                               AS VNom_AT,
    PerdaReguSec.fltVNom_BT                                               AS VNom_BT,
    PerdaReguSec.strRegional                                              AS Regional,
    PerdaReguSec.strTipologia                                             AS Tipologia,
    PerdaReguSec.strFase                                                  AS Fase,
    PerdaReguSec.strTrafo                                                 AS Trafo,
    PerdaReguSec.strCabo                                                  AS Cabo,
    PerdaReguSec.strCaboTronco                                            AS CaboTronco,
    PerdaReguSec.strCaboRamal                                             AS CaboRamal,
    PerdaReguSec.fltCompTronco                                            AS CompTronco,
    PerdaReguSec.fltCompRamal                                             AS CompRamal,
    PerdaReguSec.fltCompVaoMedio                                          AS VaoMedio,
    PerdaReguSec.bolFlow                                                  AS Flow,
    TipoRede.intTipoRedeId                                                AS TipoRedeId,
    TipoRede.strCodigo                                                    AS TipoRedeCodigo,
    IIF(ISNULL(GrupoPerdaRede.intGrupoId), 3, GrupoPerdaRede.intGrupoId)  AS GrupoPerdaRedeId,
    Rede.intRedeSupridoraId                                               AS SupridoraId,
    Rede.strCodigo                                                        AS RedeCodigo,
    RedeSupridora.strCodigo                                               AS RedeSupridoraCodigo
  FROM
    (
    (
    (
    (
    PerdaReguSec INNER JOIN Rede ON
        PerdaReguSec.intRedeId = Rede.intRedeId
    )
    INNER JOIN Rede RedeSupridora ON
      Rede.intRedeSupridoraId = RedeSupridora.intRedeId
    )
    INNER JOIN TipoRede ON
      Rede.intTipoRedeId = TipoRede.intTipoRedeId
    )
    LEFT OUTER JOIN GrupoPerdaRede ON
      PerdaReguSec.intRedeId = GrupoPerdaRede.intRedeId
    )
  WHERE
    PerdaReguSec.dttData = [p_Data];

