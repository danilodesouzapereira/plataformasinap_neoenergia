  SELECT DISTINCT
    Barra.intBarraId                                                          AS BarraId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    RREquipamentoBarra.intMRedeId                                             AS MRedeId,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    Barra.fltVNom                                                             AS VNom,
    Barra.intEsqX1                                                            AS EsqX1,
    Barra.intEsqY1                                                            AS EsqY1,
    Barra.intEsqX2                                                            AS EsqX2,
    Barra.intEsqY2                                                            AS EsqY2,
    Barra.intUtmX                                                             AS UtmX,
    Barra.intUtmY                                                             AS UtmY
  FROM
    (
    (
    RREquipamentoBarra
      INNER JOIN Equipamento ON
        RREquipamentoBarra.intEstudoId = Equipamento.intEstudoId AND
        RREquipamentoBarra.intBarraId = Equipamento.intEquipId
    )

      INNER JOIN Barra ON
        RREquipamentoBarra.intEstudoId = Barra.intEstudoId AND
        RREquipamentoBarra.intBarraId = Barra.intBarraId
    )
  WHERE
    RREquipamentoBarra.intEstudoId = [p_EstudoId] AND
    (( [p_MRedeId] = -1) OR (RREquipamentoBarra.intMRedeId = [p_MRedeId] )) AND
    (( [p_CorteId] = -1) OR (RREquipamentoBarra.intCorteId = [p_CorteId] ));
