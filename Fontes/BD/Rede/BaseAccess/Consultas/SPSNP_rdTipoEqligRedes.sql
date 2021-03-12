    SELECT
      DISTINCT(Equipamento.intRedeId) AS RedeId
    FROM
      Equipamento
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.intTipoEquipId = [p_TipoEquipId]
