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
    EqAux.intTrafoId                                                          AS TrafoId,
    EqAux.bolAutoTrafo                                                        AS AutoTrafo,
    EqAux.fltPerdaFerro                                                       AS PerdaFerro,
    EqAux.intLtcTipoUtl                                                       AS LtcTipoUtl,
    EqAux.intLtcBarRefId                                                      AS LtcBarRefId,
    EqAux.fltLtcVpu                                                           AS LtcVpu,
    EqAux.fltZ0R                                                              AS Z0R,
    EqAux.fltZ0X                                                              AS Z0X,
    EqAux.fltZ1R                                                              AS Z1R,
    EqAux.fltZ1X                                                              AS Z1X,
    TrafoEnrol.intTrafoEnrolId                                                AS Enrol_Id,
    TrafoEnrol.intTipoLigacaoId                                               AS Enrol_TipoLigacao,
    TrafoEnrol.intTipoFaseId                                                  AS Enrol_TipoFase,
    TrafoEnrol.fltVnom                                                        AS Enrol_Vnom,
    TrafoEnrol.fltSnom                                                        AS Enrol_Snom,
    TrafoEnrol.fltRotFase                                                     AS Enrol_RotFase,
    TrafoEnrol.fltVtap                                                        AS Enrol_Vtap,
    TrafoEnrol.fltZterraR                                                     AS Enrol_ZterraR,
    TrafoEnrol.fltZterraX                                                     AS Enrol_ZterraX
  FROM
    (
    (
    (
    (
    Equipamento
      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )

      INNER JOIN Trafo EqAux ON
        Equipamento.intEquipId = EqAux.intTrafoId AND
        Equipamento.intEstudoId = EqAux.intEstudoId
    )

      INNER JOIN TrafoEnrol ON
        EqAux.intTrafoId = TrafoEnrol.intTrafoId AND
        EqAux.intEstudoId = TrafoEnrol.intEstudoId
    )
      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 24 AND
    EquipamentoBarra.intBarraId = TrafoEnrol.intBarraId
  ORDER BY
    Equipamento.intEquipId,
    TrafoEnrol.intTrafoEnrolId;
