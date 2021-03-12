  SELECT
    Equipamento.intRedeId                                                     AS RedeId,
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    Equipamento.bolAlteradoObra                                               AS AlteradoObra,
    Equipamento.intShow                                                       AS Show,
    EquipamentoBarra.intBarraId                                               AS BarraId,
    EquipamentoBarra.fltPosBar                                                AS PosBar,
    EqAux.intTrafo3EId                                                        AS Trafo3EId,
    EqAux.fltPerdaFerro                                                       AS PerdaFerro,
    EqAux.fltSbase                                                            AS Sbase,
    EqAux.fltZPS0R                                                            AS ZPS0R,
    EqAux.fltZPS0X                                                            AS ZPS0X,
    EqAux.fltZPT0R                                                            AS ZPT0R,
    EqAux.fltZPT0X                                                            AS ZPT0X,
    EqAux.fltZST0R                                                            AS ZST0R,
    EqAux.fltZST0X                                                            AS ZST0X,
    EqAux.fltZPS1R                                                            AS ZPS1R,
    EqAux.fltZPS1X                                                            AS ZPS1X,
    EqAux.fltZPT1R                                                            AS ZPT1R,
    EqAux.fltZPT1X                                                            AS ZPT1X,
    EqAux.fltZST1R                                                            AS ZST1R,
    EqAux.fltZST1X                                                            AS ZST1X,
    TrafoEnrol.intTrafoEnrolId                                                AS Enrol_Id,
    TrafoEnrol.intTipoLigacaoId                                               AS Enrol_TipoLigacao,
    TrafoEnrol.intTipoFaseId                                                  AS Enrol_TipoFase,
    TrafoEnrol.fltVnom                                                        AS Enrol_Vnom,
    TrafoEnrol.fltSnom                                                        AS Enrol_Snom,
    TrafoEnrol.fltRotFase                                                     AS Enrol_RotFase,
    TrafoEnrol.fltVtap                                                        AS Enrol_Vtap,
    TrafoEnrol.fltZTerraR                                                     AS Enrol_ZterraR,
    TrafoEnrol.fltZTerraX                                                     AS Enrol_ZterraX
  FROM
    (
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )

      INNER JOIN Trafo3E EqAux ON
        Equipamento.intEstudoId = EqAux.intEstudoId AND
        Equipamento.intEquipId = EqAux.intTrafo3EId
    )

      INNER JOIN TrafoEnrol ON
        EqAux.intEstudoId = TrafoEnrol.intEstudoId AND
        EqAux.intTrafo3EId = TrafoEnrol.intTrafoId
    )
    
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 25 AND
    EquipamentoBarra.intBarraId = TrafoEnrol.intBarraId
  ORDER BY
    Equipamento.intEquipId,
    TrafoEnrol.intTrafoEnrolId;
