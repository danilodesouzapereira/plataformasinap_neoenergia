  DELETE FROM Arranjo       WHERE intArranjoId IN (
                                                  SELECT
                                                    Arranjo.intArranjoId
                                                  FROM
                                                      Arranjo LEFT OUTER JOIN Trecho ON Arranjo.intArranjoId = Trecho.intArranjoId
                                                  WHERE
                                                    Arranjo.bolTipico = 1 AND
                                                    Trecho.intTrechoId IS NULL
                                                  );

