  SELECT DISTINCT
    Barra.intBarraId                                                          AS BarraId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    Equipamento.intShow                                                       AS Show,
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
    (
    RREquipamentoBarra
      INNER JOIN Equipamento ON
        RREquipamentoBarra.intBarraId = Equipamento.intEquipId AND
        RREquipamentoBarra.intEstudoId = Equipamento.intEstudoId
    )

      LEFT OUTER JOIN TmpEstudoRedeCorte ON
        RREquipamentoBarra.intMRedeId = TmpEstudoRedeCorte.intMRedeId AND
        RREquipamentoBarra.intCorteId = TmpEstudoRedeCorte.intCorteId AND
        RREquipamentoBarra.intEstudoId = TmpEstudoRedeCorte.intEstudoId
    )

      INNER JOIN Barra ON
        RREquipamentoBarra.intBarraId = Barra.intBarraId AND
        RREquipamentoBarra.intEstudoId = Barra.intEstudoId
    )
  WHERE
    RREquipamentoBarra.intEstudoId = [p_EstudoId] AND
    (
    ((TmpEstudoRedeCorte.intMRedeId IS NULL) AND ([p_SemFiltro] = 1))  OR
    ((TmpEstudoRedeCorte.intMRedeId IS NOT NULL) AND ([p_SemFiltro] = 0))
    )
