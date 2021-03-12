DELETE 
  RRCarga.*
FROM 
  RRCarga
WHERE 
  RRCarga.intEstudoId = [p_EstudoId] AND
  ((RRCarga.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));
