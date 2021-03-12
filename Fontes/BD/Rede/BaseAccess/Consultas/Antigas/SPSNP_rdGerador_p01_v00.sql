    SELECT
      Equipamento.strCodigo                                                     AS Codigo,
      Equipamento.strExtId                                                      AS ExtId,
      IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
      IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
      IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
      Equipamento.bolAlteradoObra                                               AS AlteradoObra,
      EquipamentoBarra.intBarraId                                               AS BarraId,
      EquipamentoBarra.fltPosBar                                                AS PosBar,
      EquipamentoBarra.intDX                                                    AS DX,
      EquipamentoBarra.intDY                                                    AS DY,
      EqAux.intGeradorId                                                        AS GeradorId,
      EqAux.intTipoLigacaoId                                                    AS TipoLigacaoId,
      EqAux.intTipoSuprId                                                       AS TipoSuprId,
      EqAux.intTipoGerId                                                        AS TipoGerId,
      EqAux.fltVnom                                                             AS Vnom,
      EqAux.fltSnom                                                             AS Snom,
      EqAux.fltSmax                                                             AS Smax,
      EqAux.fltFatorPotMin                                                      AS FatorPotMin,
      EqAux.fltZ0R                                                              AS Z0R,
      EqAux.fltZ0X                                                              AS Z0X,
      EqAux.fltZ1R                                                              AS Z1R,
      EqAux.fltZ1X                                                              AS Z1X,
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

        INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
      )

        INNER JOIN ConjuntoCurva ON
          Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN Gerador EqAux ON
          Equipamento.intEstudoId = EqAux.intEstudoId AND
          Equipamento.intEquipId = EqAux.intGeradorId
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
      Equipamento.intTipoEquipId = 10 AND
      ConjuntoCurvaSegmento.intCurvaId = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1 
    ORDER BY
      Equipamento.intEquipId;
