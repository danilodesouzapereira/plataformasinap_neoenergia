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
      EqAux.intCapacitorId                                                      AS CapacitorId,
      EqAux.intTipoLigacaoId                                                    AS TipoLigacaoId,
      EqAux.fltQnom                                                             AS Qnom,
      EqAux.fltVnom                                                             AS Vnom,
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
      Equipamento
        INNER JOIN EquipamentoBarra ON
          Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
          Equipamento.intEquipId = EquipamentoBarra.intEquipId
      )

        INNER JOIN Capacitor EqAux ON
          Equipamento.intEquipId = EqAux.intCapacitorId AND
          Equipamento.intEstudoId = EqAux.intEstudoId
      )

        INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
      )

        INNER JOIN ConjuntoCurva ON
          Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN ConjuntoCurvaSegmento ON
          ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
      )

        INNER JOIN TmpEquip ON
          BarraRede.intRedeId = TmpEquip.intEquipId
      ) 

    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 5 AND
      ConjuntoCurvaSegmento.intCurvaId = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1 
    ORDER BY
      Equipamento.intEquipId;
