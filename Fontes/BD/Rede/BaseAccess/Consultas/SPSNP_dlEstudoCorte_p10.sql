DELETE 
  RRSuprimento.*
FROM 
  RRSuprimento
WHERE 
  RRSuprimento.intEstudoId = [p_EstudoId] AND
  ((RRSuprimento.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));