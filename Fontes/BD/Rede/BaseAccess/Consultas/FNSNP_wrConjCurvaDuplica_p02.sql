INSERT INTO
  ConjuntoCurvaSegmento (intConjCurvaId, intCurvaId, intSegmentoId, intTipoCurvaId, blbPontos)
SELECT 
  [p_ConjCurvaIdProx], intCurvaId, intSegmentoId, intTipoCurvaId, blbPontos
FROM
  ConjuntoCurvaSegmento
WHERE
  intConjCurvaId = [p_ConjCurvaId]