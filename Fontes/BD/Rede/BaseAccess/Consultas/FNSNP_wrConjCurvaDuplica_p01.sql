INSERT INTO
  ConjuntoCurva (intConjCurvaId, strDescricao, bolTipica, intStatus)
SELECT 
  [p_ConjCurvaIdProx], strDescricao, bolTipica, intStatus
FROM 
  ConjuntoCurva 
WHERE 
  intConjCurvaId = [p_ConjCurvaId]