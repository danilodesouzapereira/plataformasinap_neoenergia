SELECT
  Manobra.intManobraId     AS ManobraId,
  Manobra.strCodigo        AS Codigo,
  ManobraChave.intEstudoId AS EstudoId,
  ManobraChave.intChaveId  AS ChaveId,
  Equipamento.intRedeId    AS RedeId,
  ManobraChave.bolFechada  AS Fechada
FROM
  (
  (
  Manobra 
    INNER JOIN ManobraChave ON
      Manobra.intManobraId = ManobraChave.intManobraId
  )
    INNER JOIN Equipamento ON
      ManobraChave.intEstudoId = Equipamento.intEstudoId AND
      ManobraChave.intChaveId  = Equipamento.intEquipId
  )
ORDER BY
   Manobra.intManobraId;
