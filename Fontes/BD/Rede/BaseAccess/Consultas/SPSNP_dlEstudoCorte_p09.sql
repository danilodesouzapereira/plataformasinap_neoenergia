DELETE 
  RRTrafo.*
FROM 
  RRTrafo
WHERE 
  RRTrafo.intEstudoId = [p_EstudoId] AND
  ((RRTrafo.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));