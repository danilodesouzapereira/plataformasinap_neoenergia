    SELECT DISTINCT
      Equipamento.intEquipId                                                    AS BarraId,
      Equipamento.strCodigo                                                     AS Codigo,
      Equipamento.strExtId                                                      AS ExtId,
      Equipamento.bolAlteradoObra                                               AS AlteradoObra,
      Equipamento.intShow                                                       AS Show,
      IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
      IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
      IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
      EqAux.fltVNom                                                             AS VNom,
      EqAux.intEsqX1                                                            AS EsqX1,
      EqAux.intEsqY1                                                            AS EsqY1,
      EqAux.intEsqX2                                                            AS EsqX2,
      EqAux.intEsqY2                                                            AS EsqY2,
      EqAux.intUtmX                                                             AS UtmX,
      EqAux.intUtmY                                                             AS UtmY
    FROM
      (
      (
      (
      Equipamento
        INNER JOIN BarraRede ON
          Equipamento.intEstudoId = BarraRede.intEstudoId AND
          Equipamento.intEquipId = BarraRede.intBarraId
      )

        INNER JOIN Barra EqAux ON
          Equipamento.intEquipId = EqAux.intBarraId AND
          Equipamento.intEstudoId = EqAux.intEstudoId
      )

        INNER JOIN TmpEquip ON
          BarraRede.intRedeId = TmpEquip.intEquipId
      ) 

    WHERE
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      EqAux.intEstudoId = [p_EstudoId] AND
      Equipamento.intTipoEquipId = 2

