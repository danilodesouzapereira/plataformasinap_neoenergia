    UPDATE
      (SELECT * FROM ConjuntoCurvaSegmento) sel
    SET
      sel.intTipoCurvaId = [p_TipoCurvaId],
      sel.blbPontos = [p_Pontos]
    WHERE
      sel.intConjCurvaId = [p_ConjCurvaId] AND
      sel.intCurvaId = [p_CurvaId] AND
      sel.intSegmentoId = [p_SegmentoId];
