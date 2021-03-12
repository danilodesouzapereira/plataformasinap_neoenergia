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
    EqAux.intChaveId                                                          AS ChaveId,
    EqAux.intTipoChaveId                                                      AS TipoChaveId,
    EqAux.intEstado                                                           AS Estado,
    EqAux.fltInom                                                             AS Inom
  FROM
    (
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )

      INNER JOIN Chave EqAux ON
        Equipamento.intEstudoId = EqAux.intEstudoId AND
        Equipamento.intEquipId = EqAux.intChaveId
    )
    
      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 

  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 9
  ORDER BY
    Equipamento.intEquipId;
