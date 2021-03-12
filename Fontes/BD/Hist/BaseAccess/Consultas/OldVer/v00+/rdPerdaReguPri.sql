  SELECT
    PerdaReguPri.intRedeId                                                AS RedeId,
    PerdaReguPri.dttData                                                  AS Data,
    PerdaReguPri.fltFatPotencia                                           AS FatPotencia,
    PerdaReguPri.fltFatCarga                                              AS FatCarga,
    PerdaReguPri.fltFatPerda                                              AS FatPerda,
    PerdaReguPri.fltComprimento                                           AS Comprimento,
    PerdaReguPri.fltDvMax                                                 AS DvMax,
    PerdaReguPri.fltVNom                                                  AS VNom,
    PerdaReguPri.fltAngulo                                                AS Angulo,
    PerdaReguPri.fltDemandaMax                                            AS DemandaMax,
    PerdaReguPri.fltAlpha                                                 AS Alpha,
    PerdaReguPri.fltDEC                                                   AS ValDEC,
    PerdaReguPri.fltDEC                                                   AS ValFEC,
    PerdaReguPri.fltTaxaArbor                                             AS TaxaArbor,
    PerdaReguPri.fltTaxaReforma                                           AS TaxaReforma,
    PerdaReguPri.fltTaxaCrescVert                                         AS TaxaCrescVert,
    PerdaReguPri.fltTaxaCrescHorz                                         AS TaxaCrescHorz,
    PerdaReguPri.fltTaxaFalhaArbor                                        AS TaxaFalhaArbor,
    PerdaReguPri.fltTaxaFalhaEqpto                                        AS TaxaFalhaEqpto,
    PerdaReguPri.fltTaxaFalhaOutro                                        AS TaxaFalhaOutro,
    PerdaReguPri.fltAreaAtuacao                                           AS AreaAtuacao,
    PerdaReguPri.fltDistBaricentro                                        AS DistBaricentro,
    PerdaReguPri.strFase                                                  AS Fase,
    PerdaReguPri.strCaboTronco                                            AS CaboTronco,
    PerdaReguPri.strCaboTronco_3F                                         AS CaboTronco_3F,
    PerdaReguPri.strCaboTronco_2F                                         AS CaboTronco_2F,
    PerdaReguPri.strCaboTronco_1F                                         AS CaboTronco_1F,
    PerdaReguPri.strCaboRamal                                             AS CaboRamal,
    PerdaReguPri.strCaboRamal_3F                                          AS CaboRamal_3F,
    PerdaReguPri.strCaboRamal_2F                                          AS CaboRamal_2F,
    PerdaReguPri.strCaboRamal_1F                                          AS CaboRamal_1F,
    PerdaReguPri.fltCompTronco_3F                                         AS CompTronco_3F,
    PerdaReguPri.fltCompTronco_2F                                         AS CompTronco_2F,
    PerdaReguPri.fltCompTronco_1F                                         AS CompTronco_1F,
    PerdaReguPri.fltCompRamal_3F                                          AS CompRamal_3F,
    PerdaReguPri.fltCompRamal_2F                                          AS CompRamal_2F,
    PerdaReguPri.fltCompRamal_1F                                          AS CompRamal_1F,
    PerdaReguPri.intNumPontoCarga                                         AS NumPontoCarga,
    PerdaReguPri.intNumConsumidor                                         AS NumConsumidor,
    PerdaReguPri.intNumConsumidor_MT                                      AS NumConsumidor_MT,
    PerdaReguPri.intNumConsumidor_BT                                      AS NumConsumidor_BT,
    PerdaReguPri.intNumRegTensao                                          AS NumRegTensao,
    PerdaReguPri.bolFlow                                                  AS Flow,
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
    PerdaReguPri INNER JOIN Rede ON
        PerdaReguPri.intRedeId = Rede.intRedeId
    )
    INNER JOIN Rede RedeSupridora ON
      Rede.intRedeSupridoraId = RedeSupridora.intRedeId
    )
    INNER JOIN TipoRede ON
      Rede.intTipoRedeId = TipoRede.intTipoRedeId
    )
    LEFT OUTER JOIN GrupoPerdaRede ON
      PerdaReguPri.intRedeId = GrupoPerdaRede.intRedeId
    )
  WHERE
    PerdaReguPri.dttData = [p_Data];
