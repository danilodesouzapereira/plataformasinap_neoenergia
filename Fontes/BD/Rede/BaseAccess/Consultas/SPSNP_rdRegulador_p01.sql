  SELECT
    Equipamento.intRedeId                                                     AS RedeId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    Equipamento.intShow                                                       AS Show,
    EquipamentoBarra.intBarraId                                               AS BarraId,
    EquipamentoBarra.fltPosBar                                                AS PosBar,
    EqAux.intReguladorId                                                      AS ReguladorId,
    EqAux.intTipoLigacaoId                                                    AS TipoLigacaoId,
    EqAux.fltSnom                                                             AS Snom,
    EqAux.bolBypass                                                           AS Bypass,
    EqAux.fltFaixaVar                                                         AS FaixaVar,
    EqAux.intNumPasso                                                         AS NumPasso,
    EqAux.bolSentidoReversivel                                                AS SentidoReversivel,
    IIF(ISNULL(EqAux.intSentidoBarRefId), -1, EqAux.intSentidoBarRefId)       AS SentidoBarRefId,
    EqAux.bolAjusteAuto                                                       AS AjusteAuto,
    EqAux.intCfgFixPassoSel                                                   AS CfgFixPassoSel,
    IIF(ISNULL(EqAux.intCfgAutBarRefId), -1, EqAux.intCfgAutBarRefId)         AS CfgAutBarRefId,
    EqAux.fltCfgAutVpu                                                        AS CfgAutVpu,
    EqAux.fltZ0R                                                              AS Z0R,
    EqAux.fltZ0X                                                              AS Z0X,
    EqAux.fltZ1R                                                              AS Z1R,
    EqAux.fltZ1X                                                              AS Z1X,
    EqAux.intModeloId                                                         AS ModeloId,
    EqAux.intTipoFaseId                                                       AS TipoFaseId,
	EqAux.intCfgAutBarRefInvId                                                AS CfgAutBarRefInvId,
    EqAux.fltCfgAutVinvpu                                                     AS CfgAutVinvpu,
    EqAux.fltCfgAutLbmpu                                                      AS CfgAutLbmpu,
    EqAux.fltCfgAutLbmInvpu                                                   AS CfgAutLbmInvpu,
    EqAux.intRelTap                                                           AS RelTap
  FROM
    (
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEquipId = EquipamentoBarra.intEquipId AND
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId
    )
      INNER JOIN Regulador EqAux ON
        Equipamento.intEquipId = EqAux.intReguladorId AND
        Equipamento.intEstudoId = EqAux.intEstudoId
    )
      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 21
  ORDER BY
    EqAux.intReguladorId;
