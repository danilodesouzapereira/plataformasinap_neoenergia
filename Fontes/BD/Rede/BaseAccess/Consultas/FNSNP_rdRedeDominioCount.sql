    SELECT DISTINCT
      COUNT(intDominioId)
    FROM
      RedeDominio
    WHERE
      intEstudoId = [p_EstudoId] AND
      intRedeId IN (
                    SELECT
                      intRedeId
                    FROM
                      RedeDominio
                    WHERE
                      intEstudoId = [p_EstudoId] AND
                      intDominioId = [p_DominioId]
                   )
