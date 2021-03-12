SELECT
  IIF(ISNULL(MAX(intCurvaId)), 0, MAX(intCurvaId)) + 1 AS IdProx 
FROM 
  ConjuntoCurvaSegmento
WHERE 
  intConjCurvaId = [p_ConjCurvaId];
 