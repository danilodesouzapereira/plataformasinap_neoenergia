    SELECT
       DISTINCT(IIF(ISNULL(COUNT(Equipamento.intEquipId)), 0, COUNT(Equipamento.intEquipId))) AS TipoEquipCount
    FROM
      (
      (
      Equipamento
        INNER JOIN EquipamentoBarra ON
          Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND
          Equipamento.intEquipId = EquipamentoBarra.intEquipId
      )

        INNER JOIN BarraRede ON
          EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND
          EquipamentoBarra.intBarraId = BarraRede.intBarraId
      )
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      Equipamento.intTipoEquipId = [p_TipoEquipId] AND
      BarraRede.intRedeId = [p_RedeId]
