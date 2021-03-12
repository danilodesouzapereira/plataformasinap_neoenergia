  SELECT
    Equipamento.intRedeId                                                     AS RedeId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    EquipamentoBarra.intBarraId                                               AS BarraId,
    EquipamentoBarra.fltPosBar                                                AS PosBar,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    EqAux.intCapacitorSerieId                                                 AS CapacitorSerieId,
    EqAux.fltVnom                                                             AS Vnom,
    EqAux.fltSnom                                                             AS Snom,
    EqAux.fltZR                                                               AS ZR,
    EqAux.fltZX                                                               AS ZX
  FROM
    (
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )

      INNER JOIN CapacitorSerie EqAux ON
        Equipamento.intEstudoId = EqAux.intEstudoId AND
        Equipamento.intEquipId = EqAux.intCapacitorSerieId
    )

      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 

  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 6
  ORDER BY
    Equipamento.intEquipId;
