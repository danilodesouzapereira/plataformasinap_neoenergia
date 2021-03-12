DELETE 
  RREquipamentoBarra.*
FROM 
  RREquipamentoBarra
WHERE 
  RREquipamentoBarra.intEstudoId = [p_EstudoId] AND
  ((RREquipamentoBarra.intMRedeId = [p_RedeId]) OR ([p_RedeId] = -1));