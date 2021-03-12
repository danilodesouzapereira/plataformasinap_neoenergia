   SELECT
     Arranjo.intArranjoId AS ArranjoId,
     Arranjo.strDescricao AS ArranjoDescricao
   FROM
     (
     (
     ARRANJO_00_01
       INNER JOIN Arranjo ON
         ARRANJO_00_01.intArranjoId = Arranjo.intArranjoId
     )
       INNER JOIN Suporte ON
         ARRANJO_00_01.intSuporteId = Suporte.intSuporteId
     )
   WHERE
     Suporte.intSuporteId = [p_SuporteId]
   
   UNION

   SELECT
     Arranjo.intArranjoId AS ArranjoId,
     Arranjo.strDescricao AS ArranjoDescricao
   FROM
     (
     (
     ARRANJO_02_04
       INNER JOIN Arranjo ON
         ARRANJO_02_04.intArranjoId = Arranjo.intArranjoId
     )
       INNER JOIN Suporte ON
         ARRANJO_02_04.intSuporteId = Suporte.intSuporteId
     )
   WHERE
     Suporte.intSuporteId = [p_SuporteId]

