  SELECT
    Equipamento.intRedeId                                                     AS RedeId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
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
    EqAux.fltZ1X                                                              AS Z1X
  FROM
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )
      INNER JOIN Regulador EqAux ON
        Equipamento.intEstudoId = EqAux.intEstudoId AND
        Equipamento.intEquipId = EqAux.intReguladorId
    )
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 21 AND
    Equipamento.intRedeId = [p_RedeId]
  ORDER BY
    EqAux.intReguladorId;
