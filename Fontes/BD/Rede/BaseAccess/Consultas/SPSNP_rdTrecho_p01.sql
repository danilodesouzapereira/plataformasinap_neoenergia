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
    EquipamentoBarra.intDX                                                    AS DX,
    EquipamentoBarra.intDY                                                    AS DY,
    EqAux.intTrechoId                                                         AS TrechoId,
    EqAux.fltComprimento                                                      AS Comprimento,
    EqAux.intArranjoId                                                        AS ArranjoId,
    Arranjo.intTipoArranjoId                                                  AS TipoArranjoId,
    Arranjo.bolTipico                                                         AS Tipico
  FROM
    (
    (
    (
    (
    Equipamento
      INNER JOIN Trecho EqAux ON
        Equipamento.intEquipId = EqAux.intTrechoId AND
        Equipamento.intEstudoId = EqAux.intEstudoId
    )

      INNER JOIN EquipamentoBarra ON
        Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
        Equipamento.intEquipId = EquipamentoBarra.intEquipId
    )

      INNER JOIN Arranjo ON
        EqAux.intArranjoId = Arranjo.intArranjoId
    )
    
      INNER JOIN TmpEquip ON
        Equipamento.intRedeId = TmpEquip.intEquipId
    ) 
  WHERE
    Equipamento.intEstudoId = [p_EstudoId] AND
    Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
    Equipamento.intTipoEquipId = 28
  ORDER BY
    Equipamento.intRedeId,
    Equipamento.intEquipId;
