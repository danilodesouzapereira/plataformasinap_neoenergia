DELETE 
  RRSuprimento.*
FROM 
  RRSuprimento INNER JOIN TmpEquip ON
    RRSuprimento.intMRedeId = TmpEquip.intEquipId
WHERE 
  RRSuprimento.intEstudoId = [p_EstudoId];