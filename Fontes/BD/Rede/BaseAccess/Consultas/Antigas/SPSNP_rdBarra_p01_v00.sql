    SELECT
      Equipamento.intEquipId                                                    AS BarraId,
      Equipamento.strCodigo                                                     AS Codigo,
      Equipamento.strExtId                                                      AS ExtId,
      Equipamento.bolAlteradoObra                                               AS AlteradoObra,
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
          Equipamento.intEstudoId = EqAux.intEstudoId AND
          Equipamento.intEquipId = EqAux.intBarraId
      )

        INNER JOIN TmpEquip ON
          BarraRede.intRedeId = TmpEquip.intEquipId
      ) 
    WHERE
      EqAux.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 2

