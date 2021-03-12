DELETE * FROM 
  ConjuntoCurvaSegmento 
WHERE 
  ConjuntoCurvaSegmento.intConjCurvaId IN
                                          (
                                          SELECT 
                                            intConjCurvaId
                                          FROM
                                             TmpCurva
                                          );

