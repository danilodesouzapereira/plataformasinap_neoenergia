  SELECT
    Equipamento.strCodigo                                                     AS Codigo,
    Equipamento.strExtId                                                      AS ExtId,
    IIF(ISNULL(Equipamento.intDxComentario), -1, Equipamento.intDxComentario) AS DxComentario,
    IIF(ISNULL(Equipamento.intDyComentario), -1, Equipamento.intDyComentario) AS DyComentario,
    IIF(ISNULL(Equipamento.strComentario),   '', Equipamento.strComentario)   AS Comentario,
    RREquipamentoBarra.intMRedeId                                             AS MRedeId,
    RREquipamentoBarra.intBarraId                                             AS BarraId,
    RREquipamentoBarra.fltPosBar                                              AS PosBar,
    RREquipamentoBarra.intDX                                                  AS DX,
    RREquipamentoBarra.intDY                                                  AS DY,
    EqAux.intRRSuprimentoId                                                   AS RRSuprimentoId,
    EqAux.intTipoSuprId                                                       AS TipoSuprId,
    EqAux.fltVnom                                                             AS Vnom,
    EqAux.fltSmax                                                             AS Smax,
    EqAux.fltPcc3F                                                            AS Pcc3F,
    EqAux.fltQcc3F                                                            AS Qcc3F,
    EqAux.fltPccFT                                                            AS PccFT,
    EqAux.fltQccFT                                                            AS QccFT,
    EqAux.fltZ0REqv                                                           AS Z0REqv,
    EqAux.fltZ0XEqv                                                           AS Z0XEqv,
    EqAux.fltZ1REqv                                                           AS Z1REqv,
    EqAux.fltZ1XEqv                                                           AS Z1XEqv,
    ConjuntoCurva.bolTipica                                                   AS Tipica,
    ConjuntoCurva.intStatus                                                   AS Status,
    ConjuntoCurva.intConjCurvaId                                              AS ConjCurvaId,
    ConjuntoCurvaSegmento.blbPontos                                           AS Pontos
  FROM
    (
    (
    (
    (
    RRSuprimento EqAux
      INNER JOIN Equipamento ON
        EqAux.intEstudoId = Equipamento.intEstudoId AND
        EqAux.intRRSuprimentoId = Equipamento.intEquipId
    )

      INNER JOIN RREquipamentoBarra ON
        EqAux.intEstudoId = RREquipamentoBarra.intEstudoId AND
        EqAux.intMRedeId = RREquipamentoBarra.intMRedeId AND
        EqAux.intRRSuprimentoId = RREquipamentoBarra.intEquipId AND
        EqAux.intCorteId = RREquipamentoBarra.intCorteId
    )

      INNER JOIN ConjuntoCurva ON
        Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
    )

      INNER JOIN ConjuntoCurvaSegmento ON
        ConjuntoCurva.intConjCurvaId = ConjuntoCurvaSegmento.intConjCurvaId
    )
  WHERE
    EqAux.intEstudoId = [p_EstudoId] AND
    (( [p_MRedeId] = -1) OR (RREquipamentoBarra.intMRedeId = [p_MRedeId] )) AND
    (( [p_CorteId] = -1) OR (RREquipamentoBarra.intCorteId = [p_CorteId] )) AND
    ConjuntoCurvaSegmento.intCurvaId = 1 AND
    ConjuntoCurvaSegmento.intSegmentoId = 1 
  ORDER BY
    Equipamento.intEquipId;
