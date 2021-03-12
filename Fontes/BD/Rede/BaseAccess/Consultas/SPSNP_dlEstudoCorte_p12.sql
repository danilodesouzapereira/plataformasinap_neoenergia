DELETE 
  Corte.*
FROM 
  Corte
WHERE 
  Corte.intEstudoId = [p_EstudoId] AND
  ((Corte.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));
