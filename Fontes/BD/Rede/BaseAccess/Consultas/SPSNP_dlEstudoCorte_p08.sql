DELETE 
  RRYRef.*
FROM 
  RRYRef
WHERE 
  RRYRef.intEstudoId = [p_EstudoId] AND
  ((RRYRef.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));