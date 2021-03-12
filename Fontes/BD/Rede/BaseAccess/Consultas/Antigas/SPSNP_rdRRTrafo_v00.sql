  SELECT
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    RREquipamentoBarra.intMRedeId                                             AS MRedeId,
    RREquipamentoBarra.intBarraId                                             AS BarraId,
    RREquipamentoBarra.fltPosBar                                              AS PosBar,
    EqAux.intRRTrafoId                                                        AS RRTrafoId,
    EqAux.fltZ0R                                                              AS Z0R,
    EqAux.fltZ0X                                                              AS Z0X,
    EqAux.fltZ1R                                                              AS Z1R,
    EqAux.fltZ1X                                                              AS Z1X,
    EqAux.blbMatY                                                             AS MatY,
    EqAux.intPriBarraId                                                       AS PriBarraId,
    EqAux.intPriTipoLigacaoId                                                 AS PriTipoLigacaoId,
    EqAux.intPriTipoFaseId                                                    AS PriTipoFaseId,
    EqAux.fltPriVnom                                                          AS PriVnom,
    EqAux.fltPriSnom                                                          AS PriSnom,
    EqAux.fltPriZterraR                                                       AS PriZterraR,
    EqAux.fltPriZterraX                                                       AS PriZterraX,
    EqAux.intSecBarraId                                                       AS SecBarraId,
    EqAux.intSecTipoLigacaoId                                                 AS SecTipoLigacaoId,
    EqAux.intSecTipoFaseId                                                    AS SecTipoFaseId,
    EqAux.fltSecVnom                                                          AS SecVnom,
    EqAux.fltSecSnom                                                          AS SecSnom,
    EqAux.fltSecZterraR                                                       AS SecZterraR,
    EqAux.fltSecZterraX                                                       AS SecZterraX,
    EqAux.fltRotFase                                                          AS RotFase,
    EqAux.intModeloEqv                                                        AS intModeloEqv
  FROM
    (
    (
    RRTrafo EqAux
      INNER JOIN Equipamento ON
        EqAux.intEstudoId = Equipamento.intEstudoId AND
        EqAux.intRRTrafoId = Equipamento.intEquipId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId AND
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRTrafoId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (( [p_MRedeId] = -1) OR (RREquipamentoBarra.intMRedeId = [p_MRedeId] )) AND
    (( [p_CorteId] = -1) OR (RREquipamentoBarra.intCorteId = [p_CorteId] ))
  ORDER BY
    EqAux.intRRTrafoId;
