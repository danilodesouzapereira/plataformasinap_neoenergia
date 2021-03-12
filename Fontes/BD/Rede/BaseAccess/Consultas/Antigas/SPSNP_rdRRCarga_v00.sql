  SELECT
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
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
    RRCarga EqAux
      INNER JOIN Equipamento ON
        EqAux.intEstudoId = Equipamento.intEstudoId AND
        EqAux.intRRCargaId = Equipamento.intEquipId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId AND
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRCargaId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId
    )

      INNER JOIN ConjuntoCurva ON
        Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
    )

      INNER JOIN ConjuntoCurvaSegmento ON
        ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (( [p_MRedeId] = -1) OR (RREquipamentoBarra.intMRedeId = [p_MRedeId] )) AND
    (( [p_CorteId] = -1) OR (RREquipamentoBarra.intCorteId = [p_CorteId] )) AND
    ConjuntoCurvaSegmento.intCurvaId = 1 AND 
    ConjuntoCurvaSegmento.intSegmentoId = 1  
  ORDER BY
    Equipamento.intEquipId;
