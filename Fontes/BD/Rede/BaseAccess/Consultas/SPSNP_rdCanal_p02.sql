    SELECT
      Equipamento.strCodigo                                                     AS Codigo,
      Equipamento.strExtId                                                      AS ExtId,
      IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
      IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
      IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
      Equipamento.bolAlteradoObra                                               AS AlteradoObra,
      Equipamento.intShow                                                       AS Show,
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
      EqAux2.intTipoEquipId                                                     AS TipoEquipId
    FROM
      (    
      (    
      (    
      (
      Equipamento
        INNER JOIN Canal EqAux ON
          Equipamento.intEstudoId = EqAux.intEstudoId AND
          Equipamento.intEquipId = EqAux.intCanalId
      )

        INNER JOIN Equipamento EqAux2 ON
          EqAux.intEquipId = EqAux2.intEquipId AND
          EqAux.intEstudoId = EqAux2.intEstudoId
      )

      INNER JOIN ConjuntoCurva ON
          Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN ConjuntoCurvaSegmento ON
          ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
      )
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 32 AND
      ConjuntoCurvaSegmento.intCurvaId = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1 
    ORDER BY
      Equipamento.intEquipId;
