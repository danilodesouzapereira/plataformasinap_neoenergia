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
      EqAux.intBateriaId                                                        AS BateriaId,
      EqAux.intTipoFaseId                                                       AS TipoFaseId,
	  EqAux.intTipoOperacaoId                                                   AS TipoOperacaoId,
      EqAux.fltIctepu                                                           AS ICtePu,
      EqAux.fltZctepu                                                           AS ZCtePu,
      EqAux.fltSctepu                                                           AS SCtePu,	  
	  EqAux.intModeloCarga                                                      AS ModeloCarga,
	  EqAux.intModeloDescarga                                                   AS ModeloDescarga,	  
	  EqAux.fltPnomkW                                                           AS PnomkW,
	  EqAux.fltEnomkWh                                                          AS EnomkWh,
	  EqAux.fltCapinvkVA                                                        AS CapinvkVA,
	  EqAux.fltPociosapu                                                        AS Pociosapu,
	  EqAux.fltQociosapu                                                        AS Qociosapu,	  
	  EqAux.fltRendCargapu                                                      AS RendCargapu,
	  EqAux.fltRendDescpu                                                       AS RendDescpu,
      EqAux.fltGatilhoCargapu                                                   AS GatilhoCargapu,
	  EqAux.fltGatilhoDescpu                                                    AS GatilhoDescpu,
      ConjuntoCurva.intStatus                                                   AS Status,
      ConjuntoCurva.intConjCurvaId                                              AS ConjCurvaId,
      ConjuntoCurvaSegmento.blbPontos                                           AS Pontos
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

        INNER JOIN ConjuntoCurva ON
          Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
      )

        INNER JOIN Bateria EqAux ON
          Equipamento.intEstudoId = EqAux.intEstudoId AND
          Equipamento.intEquipId = EqAux.intBateriaId
      )

        INNER JOIN ConjuntoCurvaSegmento ON
          ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
      )
      
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.bolAlteradoObra = [p_AlteradoObra] AND
      Equipamento.intTipoEquipId = 36 AND
      ConjuntoCurvaSegmento.intCurvaId    = 1 AND
      ConjuntoCurvaSegmento.intSegmentoId = 1    
    ORDER BY
      Equipamento.intEquipId;      
