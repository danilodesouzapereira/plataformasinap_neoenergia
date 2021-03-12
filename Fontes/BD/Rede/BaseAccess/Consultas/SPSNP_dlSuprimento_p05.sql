  DELETE
    ccs.*
  FROM
    ConjuntoCurvaSegmento ccs INNER JOIN ConjuntoCurva cc ON 
      ccs.intConjCurvaId = cc.intConjCurvaId 
  WHERE 
    cc.intConjCurvaId = [p_ConjCurvaId] AND 
    cc.bolTipica = 0 
    