  SELECT
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
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
    RRYRef EqAux
      INNER JOIN Equipamento ON
        EqAux.intEstudoId = Equipamento.intEstudoId AND
        EqAux.intRRYRefId = Equipamento.intEquipId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId AND
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRYRefId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (( [p_MRedeId] = -1) OR (RREquipamentoBarra.intMRedeId = [p_MRedeId] )) AND
    (( [p_CorteId] = -1) OR (RREquipamentoBarra.intCorteId = [p_CorteId] ))
  ORDER BY
    Equipamento.intEquipId;
