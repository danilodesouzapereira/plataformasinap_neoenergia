  SELECT
    PerdaReguEtd.intRedeId                                                AS RedeId,
    PerdaReguEtd.dttData                                                  AS Data,
    PerdaReguEtd.fltFatCarga                                              AS FatCarga,
    PerdaReguEtd.fltFatUtilizacao                                         AS FatUtilizacao,
    PerdaReguEtd.intTipoId                                                AS TipoId,
    PerdaReguEtd.fltPotInstalada                                          AS PotInstalada,
    PerdaReguEtd.fltDemandaMax                                            AS DemandaMax,
    PerdaReguEtd.intNumTrafo                                              AS NumTrafo,
    PerdaReguEtd.intNumTrafoMin                                           AS NumTrafoMin,
    PerdaReguEtd.intNumTrafoMax                                           AS NumTrafoMax,
    PerdaReguEtd.intNumAlimMin                                            AS NumAlimMin,
    PerdaReguEtd.intNumAlimMax                                            AS NumAlimMax,
    PerdaReguEtd.strAlim                                                  AS Alim,
    PerdaReguEtd.fltTaxaCrescimento                                       AS TaxaCrescimento,
    PerdaReguEtd.fltVNom_AT                                               AS VNom_AT,
    PerdaReguEtd.fltVNom_BT                                               AS VNom_BT,
    PerdaReguEtd.bolFlow                                                  AS Flow,
    TipoRede.intTipoRedeId                                                AS TipoRedeId,
    TipoRede.strCodigo                                                    AS TipoRedeCodigo,
    IIF(ISNULL(GrupoPerdaRede.intGrupoId), 3, GrupoPerdaRede.intGrupoId)  AS GrupoPerdaRedeId,
    Rede.intRedeSupridoraId                                               AS SupridoraId,
    Rede.strCodigo                                                        AS RedeCodigo
  FROM
    (
    (
    (
    PerdaReguEtd INNER JOIN Rede ON
        PerdaReguEtd.intRedeId = Rede.intRedeId
    )
    INNER JOIN TipoRede ON
      Rede.intTipoRedeId = TipoRede.intTipoRedeId
    )
    LEFT OUTER JOIN GrupoPerdaRede ON
      PerdaReguEtd.intRedeId = GrupoPerdaRede.intRedeId
    )
  WHERE
    PerdaReguEtd.dttData = [p_Data];
