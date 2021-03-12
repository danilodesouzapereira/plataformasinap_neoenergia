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
    RREquipamentoBarra.intDX                                                  AS DX,
    RREquipamentoBarra.intDY                                                  AS DY,
    EqAux.intRRYRefId                                                         AS RRYRefId,
    EqAux.fltY0G                                                              AS Y0G,
    EqAux.fltY0B                                                              AS Y0B,
    EqAux.fltY1G                                                              AS Y1G,
    EqAux.fltY1B                                                              AS Y1B,
    EqAux.blbMatY                                                             AS MatY
  FROM
    (
    (
    (
    RRYRef EqAux
      INNER JOIN Equipamento ON
        EqAux.intRRYRefId = Equipamento.intEquipId AND
        EqAux.intEstudoId = Equipamento.intEstudoId
    )

      LEFT OUTER JOIN TmpEstudoRedeCorte ON
        EqAux.intMRedeId = TmpEstudoRedeCorte.intMRedeId AND
        EqAux.intCorteId = TmpEstudoRedeCorte.intCorteId AND
        EqAux.intEstudoId = TmpEstudoRedeCorte.intEstudoId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRYRefId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId AND
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (
    ((TmpEstudoRedeCorte.intMRedeId IS NULL) AND ([p_SemFiltro] = 1))  OR
    ((TmpEstudoRedeCorte.intMRedeId IS NOT NULL) AND ([p_SemFiltro] = 0))
    )
  ORDER BY
    Equipamento.intEquipId;
