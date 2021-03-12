DELETE * FROM 
  ConjuntoCurva 
WHERE 
  ConjuntoCurva.intConjCurvaId IN
                                 (
                                 SELECT 
                                   intConjCurvaId
                                 FROM
                                    TmpCurva
                                 );

