 SELECT
    Equipamento.intRedeId                                                     AS RedeId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    EqAux.intMutuaId                                                          AS MutuaId,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    Equipamento.intShow                                                       AS Show,
    EqAux.intTrecho1Id                                                        AS Trecho1Id,
    EqAux.intTrecho1BarraSntId                                                AS Trecho1BarraSntId,
    EqAux.fltTrecho1Pos                                                       AS Trecho1Pos,
    EqAux.intTrecho2Id                                                        AS Trecho2Id,
    EqAux.intTrecho2BarraSntId                                                AS Trecho2BarraSntId,
    EqAux.fltTrecho2Pos                                                       AS Trecho2Pos,
    EqAux.fltZ0R                                                              AS Z0R,
    EqAux.fltZ0X                                                              AS Z0X,
    EqAux.fltZ1R                                                              AS Z1R,
    EqAux.fltZ1X                                                              AS Z1X
  FROM
    (
    (
    Equipamento
      INNER JOIN Mutua EqAux ON
        Equipamento.intEquipId = EqAux.intMutuaId AND
        Equipamento.intEstudoId = EqAux.intEstudoId
    )
    
      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 15
  ORDER BY
    Equipamento.intEquipId;
