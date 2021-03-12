SELECT
  Sincronismo.intSincronismoId AS SincronismoId,
  Sincronismo.strCodigo        AS Codigo,
  Sincronismo.bolEnabled       AS Enabled,
  SincronismoChave.intEstudoId AS EstudoId,
  SincronismoChave.intChaveId  AS ChaveId,
  Equipamento.intRedeId        AS RedeId,
  SincronismoChave.bolFechada  AS Fechada,
  SincronismoChave.bolMaster   AS Master
FROM
  (
  (
  Sincronismo 
    INNER JOIN SincronismoChave ON
      Sincronismo.intSincronismoId = SincronismoChave.intSincronismoId
  )
    INNER JOIN Equipamento ON
      SincronismoChave.intEstudoId = Equipamento.intEstudoId AND
      SincronismoChave.intChaveId  = Equipamento.intEquipId
  )
ORDER BY
  Sincronismo.intSincronismoId;
  