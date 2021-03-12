    SELECT
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
      EqAux.intCargaId                                                          AS CargaId,
      EqAux.intTipoFaseId                                                       AS TipoFaseId,
      TipoConsumidor.intTag                                                     AS TipoConsTag,
      EqAux.fltEnergia                                                          AS Energia,
      EqAux.fltICtepu                                                           AS ICtePu,
      EqAux.fltZCtepu                                                           AS ZCtePu,
      EqAux.fltSCtepu                                                           AS SCtePu,
      EqAux.intNumConsRes                                                       AS NumConsRes,
      EqAux.intNumConsCom                                                       AS NumConsCom,
      EqAux.intNumConsInd                                                       AS NumConsInd,
      EqAux.intNumConsRur                                                       AS NumConsRur,
      EqAux.intNumConsOut                                                       AS NumConsOut,
      EqAux.intNumConsA4                                                        AS NumConsA4,
      ConjuntoCurva.bolTipica                                                   AS Tipica,
      ConjuntoCurva.intStatus                                                   AS Status,
      ConjuntoCurva.intConjCurvaId                                              AS ConjCurvaId,
      ConjuntoCurvaSegmento.blbPontos                                           AS Pontos
    FROM
      (
      (
      (
      (
      (
      (
      (
      Equipamento
        INNER JOIN EquipamentoBarra ON
          Equipamento.intEquipId = EquipamentoBarra.intEquipId AND
          Equipamento.intEstudoId = EquipamentoBarra.intEstudoId
      )

        INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
      )

        INNER JOIN Carga EqAux ON
          Equipamento.intEquipId = EqAux.intCargaId AND
          Equipamento.intEstudoId = EqAux.intEstudoId
      )

        INNER JOIN TmpEquip ON
          TmpEquip.intEquipId = BarraRede.intRedeId
      ) 

        INNER JOIN ConjuntoCurva ON
          Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN ConjuntoCurvaSegmento ON
          ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
      )

        INNER JOIN TipoConsumidor ON
          EqAux.intTipoConsId = TipoConsumidor.intTipoConsId
      )
   
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 7 AND
      ConjuntoCurvaSegmento.intCurvaId    = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1    
    ORDER BY
      Equipamento.intEquipId;
      