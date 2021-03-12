  SELECT
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    Equipamento.intShow                                                       AS Show,
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
    (
    RRTrafo EqAux
      INNER JOIN Equipamento ON
        EqAux.intRRTrafoId = Equipamento.intEquipId AND
        EqAux.intEstudoId = Equipamento.intEstudoId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRTrafoId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId AND
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId
    )

      LEFT OUTER JOIN TmpEstudoRedeCorte ON
        EqAux.intMRedeId = TmpEstudoRedeCorte.intMRedeId AND
        EqAux.intCorteId = TmpEstudoRedeCorte.intCorteId AND
        EqAux.intEstudoId = TmpEstudoRedeCorte.intEstudoId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (
    ((TmpEstudoRedeCorte.intMRedeId IS NULL) AND ([p_SemFiltro] = 1))  OR
    ((TmpEstudoRedeCorte.intMRedeId IS NOT NULL) AND ([p_SemFiltro] = 0))
    )
  ORDER BY
    EqAux.intRRTrafoId;
