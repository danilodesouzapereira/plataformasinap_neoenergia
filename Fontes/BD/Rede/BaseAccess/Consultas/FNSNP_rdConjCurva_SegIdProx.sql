SELECT
  IIF(ISNULL(MAX(intSegmentoId)), 0, MAX(intSegmentoId)) + 1 AS IdProx 
FROM 
  ConjuntoCurvaSegmento
WHERE 
  intConjCurvaId = [p_ConjCurvaId] AND
  intCurvaId = [p_CurvaId]
 