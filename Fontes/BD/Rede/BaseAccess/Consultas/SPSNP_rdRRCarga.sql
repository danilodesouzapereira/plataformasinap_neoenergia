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
    RREquipamentoBarra.intDx                                                  AS DX,
    RREquipamentoBarra.intDy                                                  AS DY,
    EqAux.intRRCargaId                                                        AS RRCargaId,
    EqAux.fltIcte_pu                                                          AS Icte_pu,
    EqAux.fltScte_pu                                                          AS Scte_pu,
    EqAux.fltZcte_pu                                                          AS Zcte_pu,
    ConjuntoCurva.bolTipica                                                   AS Tipica,
    ConjuntoCurva.intStatus                                                   AS Status,
    ConjuntoCurva.intConjCurvaId                                              AS ConjCurvaId,
    ConjuntoCurvaSegmento.blbPontos                                           AS Pontos
  FROM
    (
    (
    (
    (
    (
    RRCarga EqAux
      INNER JOIN Equipamento ON
        EqAux.intRRCargaId = Equipamento.intEquipId AND
        EqAux.intEstudoId = Equipamento.intEstudoId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRCargaId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId AND
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId
    )

      LEFT OUTER JOIN TmpEstudoRedeCorte ON
        EqAux.intMRedeId = TmpEstudoRedeCorte.intMRedeId AND
        EqAux.intCorteId = TmpEstudoRedeCorte.intCorteId AND
        EqAux.intEstudoId = TmpEstudoRedeCorte.intEstudoId
    )

      INNER JOIN ConjuntoCurva ON
        Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
    )

      INNER JOIN ConjuntoCurvaSegmento ON
        ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (
    ((TmpEstudoRedeCorte.intMRedeId IS NULL) AND ([p_SemFiltro] = 1))  OR
    ((TmpEstudoRedeCorte.intMRedeId IS NOT NULL) AND ([p_SemFiltro] = 0))
    ) AND
    ConjuntoCurvaSegmento.intCurvaId = 1 AND 
    ConjuntoCurvaSegmento.intSegmentoId = 1  
  ORDER BY
    Equipamento.intEquipId;
