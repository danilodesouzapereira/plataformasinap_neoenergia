SELECT
  IIF((IIF(ISNULL(MAX(intConjCurvaId)), 0, MAX(intConjCurvaId)) + 1) < 200, 200 + (IIF(ISNULL(MAX(intConjCurvaId)), 0, MAX(intConjCurvaId)) + 1), MAX(intConjCurvaId) + 1) AS IdProx 
FROM 
  ConjuntoCurva
WHERE
  intConjCurvaId > 200
 

