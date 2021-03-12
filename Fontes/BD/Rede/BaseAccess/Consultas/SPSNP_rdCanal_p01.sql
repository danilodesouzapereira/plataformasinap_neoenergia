    SELECT
      EquipCanal.strCodigo                                                          AS Codigo,
      EquipCanal.strExtId                                                           AS ExtId,
      IIF(ISNULL(EquipCanal.intDxComentario), -1, EquipCanal.intDxComentario) AS DxComentario,
      IIF(ISNULL(EquipCanal.intDyComentario), -1, EquipCanal.intDyComentario) AS DyComentario,
      IIF(ISNULL(EquipCanal.strComentario),   '', EquipCanal.strComentario)   AS Comentario,
      EquipCanal.bolAlteradoObra                                                    AS AlteradoObra,
      EquipCanal.intShow                                                            AS Show,
      EqAux.intMedidorId                                                        AS MedidorId,
      EqAux.intCanalId                                                          AS CanalId,
      EqAux.fltSigma                                                            AS Sigma,
      EqAux.intTipoCanalId                                                      AS TipoCanalId,
      ConjuntoCurva.bolTipica                                                   AS Tipica,
      ConjuntoCurva.intStatus                                                   AS Status,
      ConjuntoCurva.intConjCurvaId                                              AS ConjCurvaId,
      ConjuntoCurvaSegmento.blbPontos                                           AS Pontos,
      EqAux.intEquipId                                                          AS EquipId,
      EqAux.intTipoFaseId                                                       AS TipoFaseId,
      EquipVincCanal.intTipoEquipId                                                     AS TipoEquipId
    FROM
      (
      (
      (
      (
      (
      (
      (
      (
      Equipamento EquipMedidor
        INNER JOIN EquipamentoBarra ON
          EquipMedidor.intEstudoId = EquipamentoBarra.intEstudoId AND
          EquipMedidor.intEquipId = EquipamentoBarra.intEquipId
      )

        INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
      )

        INNER JOIN TmpEquip ON
          BarraRede.intRedeId = TmpEquip.intEquipId
      ) 

        INNER JOIN Canal EqAux ON
          EquipMedidor.intEstudoId = EqAux.intEstudoId AND
          EquipMedidor.intEquipId = EqAux.intMedidorId
      )

        INNER JOIN Equipamento EquipVincCanal ON
          EqAux.intEquipId = EquipVincCanal.intEquipId AND
          EqAux.intEstudoId = EquipVincCanal.intEstudoId
      )

        INNER JOIN Equipamento EquipCanal ON
          EqAux.intCanalId = EquipCanal.intEquipId AND
          EqAux.intEstudoId = EquipCanal.intEstudoId
      )

        INNER JOIN ConjuntoCurva ON
          EquipCanal.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN ConjuntoCurvaSegmento ON
          ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
      )


    WHERE
      EquipMedidor.intEstudoId = [p_EstudoId] AND
      EquipMedidor.bolAlteradoObra = [p_AlteradoObra] AND
      EquipMedidor.intTipoEquipId = 13 AND
      ConjuntoCurvaSegmento.intCurvaId = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1 
      
      